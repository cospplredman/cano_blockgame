#include"mc_protocol.h"
#include<stdio.h>
#include<string.h>
#include<stddef.h>
#include<assert.h>
#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include"mc_pkt.h"

static int peer_getc(void *p){
	return ntwk_getc((struct ntwk_peer*)p);
}

static int peer_putchar(void *p, int v){
	return ntwk_putchar((struct ntwk_peer*)p, v);
}

static int print_putchar(void *p, int val){
	return putchar(val);
}

static struct mc_string motd = MC_STR(STR(
{
    "version": { "name": "1.8", "protocol": 47 },
    "players": {
        "max": 255,
        "online": 0
    },
    "description": {
        "text": "Cano Block Game"
    }
}
));

void handle_events47(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);

	int32_t pkt_len = read_VarInt(peer, peer_getc);
	size_t pkt_start = ntwk_peer_recvd(peer);
	int32_t packet_id = read_VarInt(peer, peer_getc);

	printf("47: packet 0x%x %d\n", packet_id, pkt_len);

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
			printf("status response: %.*s\n", motd.len, motd.str);
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
			struct mc_uuid uuid = (struct mc_uuid){rand(), rand()}; //TODO do it properly
			printf("login %.*s\n", username.len, username.str);

			cli->state = 3;

			free_mc_string(cli->name);
			cli->name = username;


			write_login_success(peer, peer_putchar, username, MC_STR("12910128-1931-1891-0983-189213281989")); //TODO less hard codey

			write_join_game(peer, peer_putchar, uuid.low);
			write_set_default_spawn_position(peer, peer_putchar, 0, 0, 0);
			write_pos_look(peer, peer_putchar, 0, 0, 0, 0, 0);
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
		case 0x0: { //keep alive
			printf("keep alive %d\n", read_VarInt(peer, peer_getc)); 
			}break;
		case 0x03: { //player
			read_uint8_t(peer, peer_getc); //on ground 
			   
			} break;
		case 0x04: { //player position
			double x = read_double(peer, peer_getc);
			double y = read_double(peer, peer_getc);
			double z = read_double(peer, peer_getc);
			uint8_t on_ground = read_uint8_t(peer, peer_getc);

			cli->x = x;
			cli->y = y;
			cli->z = z;

			printf("set player pos/rot: %f %f %f %f %f\n", cli->x, cli->y, cli->z, cli->yaw, cli->pitch);
			} break;
		case 0x05: { //player look
			float yaw = read_float(peer, peer_getc); 
			float pitch = read_float(peer, peer_getc);
			uint8_t on_ground = read_uint8_t(peer, peer_getc);

			cli->pitch = pitch;
			cli->yaw = yaw;

			printf("set player pos/rot: %f %f %f %f %f\n", cli->x, cli->y, cli->z, cli->yaw, cli->pitch);
			} break;
		case 0x06: { //player position and rotation
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
			printf("%c", cur_char);	
		}
	}
	
	if(ntwk_peer_recvd(peer) - pkt_start != pkt_len){
		write_disconnect(peer, peer_putchar, MC_STR(STR({"text": "invalid packet"})));
		ntwk_disconnect_peer(conf, peer);
	}
}

void disconnect47(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason){
	char buf[1024];
	snprintf(buf, 1023, "{\"text\":\"%s\"}", reason);
	write_disconnect(peer, peer_putchar, (struct mc_string){.str = buf, .len = strlen(buf)});
	ntwk_disconnect_peer(conf, peer);
}

void teleport47(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z){
	//TODO
}

void set_block47(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);
	if(cli->state != 3)
		return;
	write_block_change(peer, peer_putchar, x, y, z, block);
}

void keep_alive47(struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);
	if(cli->state != 3)
		return;
	write_keep_alive(peer, peer_putchar, rand());
}

void set_chunk47(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);
	if(cli->state != 3)
		return;


	//pkt len
	char *end = pkt_buf;
	write_VarInt(&end, buf_putchar, 0x21);
	write_int32_t(&end, buf_putchar, x);
	write_int32_t(&end, buf_putchar, z);
	write_uint8_t(&end, buf_putchar, 0x1); //ground up continuous
	write_uint16_t(&end, buf_putchar, 0xffff);

	write_VarInt(&end, buf_putchar, ((4096 * 2) + 2048 + 2048) * 16 + 256); //chunk data len

	for(size_t y = 0; y < 16; y++)
		for(size_t i = 0; i < 4096; i++){ //block data
			uint16_t block = ca(cb, x + ((i >> 0) & 0xf), y + ((i >> 8) & 0xf), z + ((i >> 4) & 0xf));

			block <<= 4;

			write_uint8_t(&end, buf_putchar, block & 0xff);
			write_uint8_t(&end, buf_putchar, block << 8);
		}
	
	for(size_t y = 0; y < 16; y++)
		for(size_t i = 0; i < 2048; i++)
			write_uint8_t(&end, buf_putchar, 0xff);

	for(size_t y = 0; y < 16; y++)
		for(size_t i = 0; i < 2048; i++)
			write_uint8_t(&end, buf_putchar, 0xff);
	
	for(size_t i = 0; i < 256; i++)
		write_uint8_t(&end, buf_putchar, 1);

	size_t len = end - pkt_buf;
	write_VarInt(peer, peer_putchar, len);
	for(size_t i = 0; i < len; i++)
		write_uint8_t(peer, peer_putchar, pkt_buf[i]);
}
