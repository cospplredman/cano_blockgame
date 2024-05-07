
#ifndef NTWK_HEADER_GUARD_H
#define NTWK_HEADER_GUARD_H

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>

struct ntwk_peer{
	void *data;
	int sock;
	size_t recvd;
	struct sockaddr addr;
};

struct ntwk_conf{
	void (*on_packet)(struct ntwk_conf *conf, struct ntwk_peer *peer);
	void (*on_connect)(struct ntwk_conf *conf, struct ntwk_peer *peer);
	void (*on_disconnect)(struct ntwk_conf *conf, struct ntwk_peer *peer);

	int my_sock;
	uint16_t port;
	size_t max_peers;
	size_t peers;
	struct ntwk_peer peer[];
};

inline static ssize_t ntwk_send_buf (struct ntwk_peer *peer, char *buf, size_t len_){
	ssize_t sent = 0, len = (ssize_t)len_;
	
	while(sent < len){
		ssize_t bytes = send(peer->sock, buf + sent, len - sent, 0);

		if(bytes < 1)
			return sent;

		sent += bytes;
	}

	return sent;
}


/* nagles algorithm my beloved */
inline static int ntwk_putchar(struct ntwk_peer *peer, int chr){
	char buf[1] = {chr};
	if(ntwk_send_buf(peer, buf, 1) < 1)
		return EOF;
	return (unsigned char)buf[0];
}

inline static ssize_t ntwk_recv_buf (struct ntwk_peer *peer, char *buf, size_t len_){
	ssize_t recvd = 0, len = (ssize_t)len_;
	
	while(recvd < len){
		ssize_t bytes = recv(peer->sock, buf + recvd, len - recvd, 0);

		if(bytes < 1)
			return recvd;

		recvd += bytes;
	}

	peer->recvd += recvd;
	return recvd;
}

inline static int ntwk_getc(struct ntwk_peer *peer){
	char buf[1];
	if(ntwk_recv_buf(peer, buf, 1) < 1)
		return EOF;
	return (unsigned char)buf[0];
}

inline static int ntwk_connect_peer(struct ntwk_conf *conf, char *address, int port){
	struct sockaddr_in addr_in;
	if(conf->peers >= conf->max_peers)
		return -1;

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);

	if(inet_pton(AF_INET, address, &addr_in.sin_addr) <= 0)
		return -1;

	int client_fd;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//printf("\n Socket creation error \n");
		return -1;
	}

	int T = 1;
	setsockopt(client_fd,SOL_SOCKET,SO_REUSEADDR,&T,sizeof(int));

	if (connect(client_fd, (struct sockaddr*)&addr_in, sizeof(struct sockaddr)) < 0) {
		//printf("\nConnection Failed \n");
		return -1;
	}

	struct sockaddr addr;
	socklen_t sz = sizeof(struct sockaddr);
	getpeername(client_fd, &addr, &sz);
	//printf("[*] connection to peer\n");
	conf->peer[conf->peers] = (struct ntwk_peer){
		.sock = client_fd,
		.addr = addr,
		.data = NULL
	};
	conf->peers++;

	conf->on_connect(conf, conf->peer + conf->peers - 1);
	return 0;
}

inline static void ntwk_disconnect_peer(struct ntwk_conf *conf, struct ntwk_peer *peer){
	conf->on_disconnect(conf, peer);
	int T = 1;
	close(peer->sock);

	for(struct ntwk_peer *p = peer + 1; p < conf->peer + conf->peers; p++)
		p[-1] = *p;

	conf->peers--;
}

inline static struct ntwk_conf *ntwk_init(
	uint16_t port,
	size_t max_peers,
	void (*on_packet)(struct ntwk_conf *conf, struct ntwk_peer *peer), 
	void (*on_connect)(struct ntwk_conf *conf, struct ntwk_peer *peer),
	void (*on_disconnect)(struct ntwk_conf *conf, struct ntwk_peer *peer)
){
	int server_socket;
	struct sockaddr_in server_address;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		printf("Error creating socket\n");
		return NULL;
	}

	int T = 1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&T,sizeof(int));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		//printf("Error binding socket\n");
		return NULL;
	}

	if (listen(server_socket, 5) == -1) {
		//printf("Error listening for connections\n");
		return NULL;
	}

	int flags;
	if ((flags = fcntl(server_socket, F_GETFL, 0)) == -1) {
		//printf("fcntl F_GETFL\n");
		return NULL;
	}

	if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		//printf("fcntl F_SETFL\n");
		return NULL;
	}

	struct ntwk_conf *conf = malloc(sizeof(struct ntwk_conf) + sizeof(struct ntwk_peer) * max_peers);

	*conf = (struct ntwk_conf){
		.port = port,
		.max_peers = max_peers,
		.peers = 0,
		.my_sock = server_socket,
		.on_packet = on_packet,
		.on_connect = on_connect,
		.on_disconnect = on_disconnect	
	};

	return conf;
}

inline static void ntwk_handle_connections(struct ntwk_conf *conf){
	if(conf->peers < conf->max_peers){
		struct sockaddr addr;
		socklen_t sz = sizeof(struct sockaddr);
		int fd = accept(conf->my_sock, &addr, &sz);
		if(fd >= 0){
			conf->peer[conf->peers] = (struct ntwk_peer){
				.sock = fd,
				.addr = addr,
				.data = NULL
			};

			conf->peers++;
			conf->on_connect(conf, conf->peer + conf->peers - 1);
		}
	}
}

inline static void ntwk_handle_packets(struct ntwk_conf *conf){
	char buf[1];
	for(size_t i = 0; i < conf->peers; i++){
		struct ntwk_peer *peer = &(conf->peer[i]);
		ssize_t status = recv(peer->sock, buf, 1, MSG_PEEK | MSG_DONTWAIT);

		if(status == 1)
			conf->on_packet(conf, peer);

		if(status == 0)
			ntwk_disconnect_peer(conf, peer);
	}
}

inline static void ntwk_handle_events(struct ntwk_conf *conf){
	ntwk_handle_connections(conf);
	ntwk_handle_packets(conf);
}

inline static void ntwk_deinit(struct ntwk_conf *conf){
	close(conf->my_sock);

	for(size_t i = 0; i < conf->peers; i++){
		struct ntwk_peer *peer = &(conf->peer[i]);
		ntwk_disconnect_peer(conf, peer);
	}

	free(conf);
}

inline static struct ntwk_peer *ntwk_get_peers(struct ntwk_conf *conf, size_t *len){
	*len = conf->peers;
	return conf->peer;
}

inline static void** ntwk_peer_get_data(struct ntwk_peer *peer){
	return &peer->data;
}

inline static size_t ntwk_peer_recvd(struct ntwk_peer *peer){
	return peer->recvd;
}

#endif /* NTWK_HEADER_GUARD_H */

