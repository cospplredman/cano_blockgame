#include"mc_protocol.h"
#include<stdio.h>
#include<string.h>
#include<stddef.h>
#include<assert.h>
#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include"mc_pkt.h"

static struct nbttag RegistryCodec = 
#include"mc_data/RegistryCodec.inc"
;

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

void handle_events762(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);

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

			cli->state = 3;
			cli->uuid = (struct mc_uuid){.low = rand(), .high = 57};

			free_mc_string(cli->name);
			cli->name = username;

			char *end, *dummy, *start;
			write_login_success(peer, peer_putchar, username, uuid);
			write_login(peer, peer_putchar, cli->uuid.low, RegistryCodec);
			write_set_default_spawn_position(peer, peer_putchar, 0, 0, 0, 0);
			write_player_info_update(peer, peer_putchar, cli->name, cli->uuid);

			write_set_render_dist(peer, peer_putchar, 32);
			//push_world_data(peer); //TODO
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
			printf("locale %.*s\n", str.len, str.str);
			free_mc_string(str);
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

			int32_t cx = x/16;
			int32_t cz = z/16;

			if(cx != cli->x || cz != cli->z)
				write_center_chunk(peer, peer_putchar, cx, cz);
			
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

			int32_t cx = x/16;
			int32_t cz = z/16;

			if(cx != cli->x || cz != cli->z)
				write_center_chunk(peer, peer_putchar, cx, cz);
			
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
	
	printf("\n\n");
	if(ntwk_peer_recvd(peer) - pkt_start != pkt_len){
		write_disconnect(peer, peer_putchar, MC_STR(STR({"text": "invalid packet"})));
		ntwk_disconnect_peer(conf, peer);
	}
}

void disconnect762(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason){
	char buf[1024];
	snprintf(buf, 1023, "{\"text\":\"%s\"}", reason);

	write_disconnect(peer, peer_putchar, (struct mc_string){.str = buf, .len = strlen(buf)});
	ntwk_disconnect_peer(conf, peer);
}

void teleport762(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z){
	//TODO
}

void set_block762(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block){
	//TODO
}

void keep_alive762(struct ntwk_peer *peer){
	write_keep_alive(peer, peer_putchar, rand());
}

void set_chunk762(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z){
	write_chunk_data(peer, peer_putchar, cb, ca, x, z);
}
