#include<stdio.h>
#include<unistd.h>
#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include"mc_pkt.h"
#include<stddef.h>
#include<assert.h>
#include<signal.h>

#include <sys/time.h>

struct nbttag RegistryCodec = 
#include"../mc_data/RegistryCodec.inc"
;

struct mc_chunk chunk;

// #include <time.h>
double get_time(){
        struct timeval start;
        gettimeofday(&start, NULL);
        //printf("%d %d\n", start.tv_sec, start.tv_usec);

        //TODO probably don't need this precision
        double us = (start.tv_sec*1e6 + start.tv_usec);
        return us * 1e-6;

//      return clock() / CLOCKS_PER_SEC
}


int peer_getc(void *p){
	return ntwk_getc((struct ntwk_peer*)p);
}

int peer_putchar(void *p, int v){
	return ntwk_putchar((struct ntwk_peer*)p, v);
}

int print_putchar(void *p, int val){
	return putchar(val);
}
static int flag_die = 0;
static int64_t eid = 0;
static struct mc_string motd = MC_STR(STR(
{
    "version": { "name": "1.19.4", "protocol": 762 },
    "players": {
        "max": 10,
        "online": 0,
        "sample": []
    },
    "description": {
        "text": "Cano Block Game"
    },
    "enforcesSecureChat": false,
    "previewsChat": true
}
));





void sigint_handler(int signum) {
	signal(SIGINT, sigint_handler);
	flag_die = 1;
}

struct mc_client{
	int32_t state;
	int32_t eid;

	double x, y, z;
	float pitch, yaw;

	struct mc_uuid uuid;
	struct mc_string name;
};


int32_t world_fn(void *a, int32_t x, int32_t y, int32_t z){
	double rn = (double)rand() / (double)RAND_MAX;
	if(rn < 0.1)
		return 1;
	return 0;
}

void push_world_data(struct ntwk_peer *peer){
	struct mc_client *cli = *(struct mc_client**)ntwk_peer_get_data(peer);

	int32_t cx = (int32_t)(cli->x/16), cz = (int32_t)(cli->z/16);

	write_center_chunk(peer, peer_putchar, cx, cz);
	write_set_render_dist(peer, peer_putchar, 32);
	for(int32_t i = -5; i < 6; i++)
		for(int32_t j = -5; j < 6; j++)
			write_chunk_data(peer, peer_putchar, NULL, world_fn, cx + i, cz + j);
	
}


void on_packet(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_client *cli = *(struct mc_client**)ntwk_peer_get_data(peer);

	int32_t pkt_len = read_VarInt(peer, peer_getc);
	size_t pkt_start = ntwk_peer_recvd(peer);
	int32_t packet_id = read_VarInt(peer, peer_getc);

	switch(cli->state){
	case 0: // handshaking
		switch(packet_id){
		case 0: { //handshake
			printf("protocol version %d\n", read_VarInt(peer, peer_getc));
			free_mc_string(read_mc_string(peer, peer_getc)); //server address
			read_uint16_t(peer, peer_getc); //server port
			int32_t state = read_VarInt(peer, peer_getc);
			cli->state = state;
			}break;
		default: {
			printf("unexpected packet\n"); 
			printf("packet length: %d\n", pkt_len);
			printf("packet id: 0x%x\n", packet_id);
			printf("state: %d\n\n", cli->state);

			}break;
		} break;
	case 1:  //status
		switch(packet_id){
		case 0: { //status request
			write_status_response(peer, peer_putchar, motd);
			}break;
		case 1: { //ping request
			write_ping_response(peer, peer_putchar, read_int64_t(peer, peer_getc));
			ntwk_disconnect_peer(conf, peer);
			}break;
		default: {
			printf("unexpected packet\n"); 
			printf("packet length: %d\n", pkt_len);
			printf("packet id: 0x%x\n", packet_id);
			printf("state: %d\n\n", cli->state);

			}break;
		} break;
	case 2:  //login
		switch(packet_id){
		case 0: { //login start
			struct mc_string username = read_mc_string(peer, peer_getc);
			uint8_t has_uuid = read_uint8_t(peer, peer_getc);
			struct mc_uuid uuid;
			if(has_uuid)
				uuid = read_mc_uuid(peer, peer_getc);
			else
				uuid = (struct mc_uuid){0, 0};

			printf("login %.*s %u:%u\n", username.len, username.str, uuid.low, uuid.high);

			*cli = (struct mc_client){
				.eid = eid++,
				.state = 3,
				.uuid = (struct mc_uuid){cli->eid, 57},
				.name = username
			};

			char *end, *dummy, *start;
			write_login_success(peer, peer_putchar, username, uuid);
			write_login(peer, peer_putchar, cli->eid, RegistryCodec);
			write_set_default_spawn_position(peer, peer_putchar, 0, 0, 0, 0);
			write_player_info_update(peer, peer_putchar, cli->name, cli->uuid);

			push_world_data(peer);
			}break;
		default: {
			printf("unexpected packet\n"); 
			printf("packet length: %d\n", pkt_len);
			printf("packet id: 0x%x\n", packet_id);
			printf("state: %d\n\n", cli->state);

			}break;
		} break;
	case 3:  //play
		switch(packet_id){
		case 0: { //confirm teleport
			printf("confirm teleport %d\n", read_VarInt(peer, peer_getc));
			}break;
		case 0x12: { //keep alive
			printf("keep alive %d\n", read_int64_t(peer, peer_getc)); 
			}break;
		case 0x08: { //client information
			printf("client information:");
			struct mc_string str = read_mc_string(peer, peer_getc);
			free_mc_string(str);
			printf("locale %.*s\n", str.len, str.str);
			printf("view dist %d\n", read_uint8_t(peer, peer_getc));
			printf("chat mode %d\n", read_VarInt(peer, peer_getc));
			printf("chat colors %d\n", read_uint8_t(peer, peer_getc));
			printf("displayed skin parts %d\n", read_uint8_t(peer, peer_getc));
			printf("main hand %d\n", read_VarInt(peer, peer_getc));
			printf("enable text filtering %d\n", read_uint8_t(peer, peer_getc));
			printf("allow server listings %d\n", read_uint8_t(peer, peer_getc));
			} break;
		case 0x14: { //set player position
			double x = read_double(peer, peer_getc);
			double y = read_double(peer, peer_getc);
			double z = read_double(peer, peer_getc);
			uint8_t on_ground = read_uint8_t(peer, peer_getc);

			cli->x = x;
			cli->y = y;
			cli->z = z;

			printf("set player pos/rot: %f %f %f %f %f\n", cli->x, cli->y, cli->z, cli->yaw, cli->pitch);
			} break;
		case 0x15: { //set player position and rotation
			double x = read_double(peer, peer_getc);
			double y = read_double(peer, peer_getc);
			double z = read_double(peer, peer_getc);
			float yaw = read_float(peer, peer_getc); 
			float pitch = read_float(peer, peer_getc);
			uint8_t on_ground = read_uint8_t(peer, peer_getc);
			
			cli->x = x;
			cli->y = y;
			cli->z = z;
			cli->pitch = pitch;
			cli->yaw = yaw;

			printf("set player pos/rot: %f %f %f %f %f\n", cli->x, cli->y, cli->z, cli->yaw, cli->pitch);
			}break;
		case 0x16: { //set player rotation
			float yaw = read_float(peer, peer_getc); 
			float pitch = read_float(peer, peer_getc);
			uint8_t on_ground = read_uint8_t(peer, peer_getc);

			cli->pitch = pitch;
			cli->yaw = yaw;

			printf("set player pos/rot: %f %f %f %f %f\n", cli->x, cli->y, cli->z, cli->yaw, cli->pitch);
			} break;
		case 0x1e: { //player command (crouch and such)
			int32_t eid = read_VarInt(peer, peer_getc);
			int32_t action = read_VarInt(peer, peer_getc);
			int32_t jmp_boost = read_VarInt(peer, peer_getc);
			}break;
		default: {
			printf("unexpected packet\n"); 
			printf("packet length: %d\n", pkt_len);
			printf("packet id: 0x%x\n", packet_id);
			printf("state: %d\n\n", cli->state);

			}break;
		} break;
	default: {
		printf("unexpected state: %d\n", cli->state);
		} break;
	}

	if(ntwk_peer_recvd(peer) - pkt_start < pkt_len){
		printf("did not read entire packet:\n");
		while(ntwk_peer_recvd(peer) - pkt_start < pkt_len){
			int cur_char = peer_getc(peer);
			if(cur_char == EOF)
				break;
			printf("%c", cur_char);	
		}
	}
	
	printf("\n\n");
	if(ntwk_peer_recvd(peer) - pkt_start != pkt_len){
		write_disconnect(peer, peer_putchar, MC_STR(STR({"text": "invalid packet"})));
		ntwk_disconnect_peer(conf, peer);
	}
}


void on_connect(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_client **cli = (struct mc_client**)ntwk_peer_get_data(peer);

	printf("new connection\n");
	*cli = malloc(sizeof(struct mc_client));

	**cli = (struct mc_client){};
}

void on_disconnect(struct ntwk_conf *conf, struct ntwk_peer *peer){
	printf("\ndisconnect\n");
	struct mc_client **cli = (struct mc_client**)ntwk_peer_get_data(peer);
	free(*cli);
}


int main(){

	chunk.len = 16;
	chunk.sub = malloc(sizeof(struct mc_subchunk) * chunk.len);
	for(size_t i = 0; i < chunk.len; i++){
		for(size_t j = 0; j < 4096; j++){
			chunk.sub[i].block[j] = 0;
		}
	}

	for(size_t i = 0; i < 4096; i++){
		chunk.sub->block[i] = 1;
	}

	//write_chunk_data(NULL, print_putchar, 0, 0, chunk);
	//return;

	struct ntwk_conf *conf = ntwk_init(
		25565,
		256,
		on_packet,
		on_connect,
		on_disconnect
	);

	if(conf == NULL){
		perror("ntwk");
		return EXIT_FAILURE;
	}

	signal(SIGINT, sigint_handler);
	

	double time = get_time();
	double tick = time;
	while(1){
		ntwk_handle_events(conf);
		
		if(get_time() - time > 20){
			printf("sending keep alive:\n");
			size_t peers;
			struct ntwk_peer *peer = ntwk_get_peers(conf, &peers);
			for(int i = 0; i < peers; i++)
				write_keep_alive(&(peer[i]), peer_putchar, 77);
			time = get_time();
		}

		if(tick - time > 0.1){
			size_t peers;
			struct ntwk_peer *peer = ntwk_get_peers(conf, &peers);
			for(int i = 0; i < peers; i++)
				push_world_data(&(peer[i]));
			tick = get_time();
		}

		if(flag_die)
			break;
	}

	printf("cleaning up\n");
	ntwk_deinit(conf);
}

