
#include"mc_protocol.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include"mc_pkt.h"

static int peer_getc(void *p){
	int cur_char = ntwk_getc((struct ntwk_peer*)p);

	printf("%c", cur_char);
	return cur_char;
}

static int peer_putchar(void *p, int v){
	return ntwk_putchar((struct ntwk_peer*)p, v);
}

//TODO store first few packets and parse them in both the old and new formats to see which fails
void handle_events0(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);

	int32_t pkt_len = read_VarInt(peer, peer_getc);

	if(pkt_len == 254){
		printf("0: 0x%x\n", read_uint8_t(peer, peer_getc)); //0xfa plugin message
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //MC|PingHost
		read_int16_t(peer, peer_getc); //length of data
		cli->protocol = read_uint8_t(peer, peer_getc); //protocol version
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //host name
		read_uint32_t(peer, peer_getc); //host port
		
		write_uint8_t(peer, peer_putchar, 0xff); //legacy kick
		
		//TODO lie about protocol version or add server list ping to exposed protocol functionality
		write_mc_utf16(peer, peer_putchar, (struct mc_utf16){.str = u"§1\00078\000cano\000Cano Block game\0000\000255", .len = 32, .code_points = 32});
		return;
	}

	if(pkt_len == 2){
		free_mc_string(cli->name);

		cli->protocol = read_uint8_t(peer, peer_getc); //protocol version
		struct mc_utf16 name = read_mc_utf16(peer, peer_getc);

		if(name.code_points != name.len)
			printf("unexpected code points");

		cli->name.str = malloc(name.code_points);
		cli->name.len = name.code_points;
		for(size_t i = 0; i < name.code_points; i++){
			cli->name.str[i] = name.str[i];
		}

		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //server host
		read_int32_t(peer, peer_getc); //server port
		return;		
	}

	size_t pkt_start = ntwk_peer_recvd(peer);
	int32_t packet_id = read_VarInt(peer, peer_getc);

	switch(cli->state){
	case 0: // handshaking
		switch(packet_id){
		case 0: { //handshake
			cli->protocol = read_VarInt(peer, peer_getc);
			free_mc_string(read_mc_string(peer, peer_getc));
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
	default: {
		printf("unexpected state: %d\n", cli->state);
		ntwk_disconnect_peer(conf, peer);
		return;
		} break;
	}

	if(ntwk_peer_recvd(peer) - pkt_start < pkt_len){
		printf("did not read entire packet:\n");
		while(ntwk_peer_recvd(peer) - pkt_start < pkt_len){
			int cur_char = peer_getc(peer);
			if(cur_char == EOF)
				break;
		}
	}
	
	if(ntwk_peer_recvd(peer) - pkt_start != pkt_len){
		ntwk_disconnect_peer(conf, peer);
	}
}

void disconnect0(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason){
	ntwk_disconnect_peer(conf, peer);
}

void teleport0(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z){

}

void set_block0(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block){

}

void keep_alive0(struct ntwk_peer *peer){

}

void set_chunk0(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z){

}
