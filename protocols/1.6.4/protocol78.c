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

void handle_events78(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);

	uint8_t packet_id = read_uint8_t(peer, peer_getc);

	if(cli->state == 0){
		cli->state = 1;
		write_login_request(peer, peer_putchar);
	}

	printf("78: packet id: 0x%x\n", packet_id);

	switch(packet_id){
	case 0x00: {//Keep Alive
		read_int32_t(peer, peer_getc);
		} break;
	case 0x01: { //login request (sent by modded clients)
		read_uint32_t(peer, peer_getc);
		free_mc_utf16(read_mc_utf16(peer, peer_getc));
		read_uint8_t(peer, peer_getc);
		read_uint8_t(peer, peer_getc);
		read_uint8_t(peer, peer_getc);
		read_uint8_t(peer, peer_getc);
		read_uint8_t(peer, peer_getc);
		} break;
	case 0x02: { //handshake
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
		} break;
	case 0x03: { //chat message
		free_mc_utf16(read_mc_utf16(peer, peer_getc));
		} break;
	case 0x07: {
		read_int32_t(peer, peer_getc); // user
		read_int32_t(peer, peer_getc); // target
		read_uint8_t(peer, peer_getc); // mouse button
		} break; //use entity
	case 0x0a: { // player
		read_uint8_t(peer, peer_getc); // on ground
		} break;
	case 0x0b: { //player position
		cli->x = read_double(peer, peer_getc);
		cli->y = read_double(peer, peer_getc);
		read_double(peer, peer_getc); //stance (crouching)
		cli->z = read_double(peer, peer_getc);
		read_uint8_t(peer, peer_getc); //on ground
		} break;
	case 0x0c: { //player look
		cli->yaw = read_float(peer, peer_getc);
		cli->pitch = read_float(peer, peer_getc);
		read_uint8_t(peer, peer_getc); //on ground
		} break;
	case 0x0d: { //player position and look
		cli->x = read_double(peer, peer_getc);
		cli->y = read_double(peer, peer_getc);
		read_double(peer, peer_getc); //stance (crouching)
		cli->z = read_double(peer, peer_getc);
		cli->yaw = read_float(peer, peer_getc);
		cli->pitch = read_float(peer, peer_getc);
		read_uint8_t(peer, peer_getc); //on ground
		} break;
	case 0x0e: { //player digging
		read_uint8_t(peer, peer_getc); //status
		read_int32_t(peer, peer_getc); //X
		read_uint8_t(peer, peer_getc); //Y
		read_int32_t(peer, peer_getc); //Z
		read_uint8_t(peer, peer_getc); //face
		} break;
	case 0x0f: { //player block placement
		read_int32_t(peer, peer_getc); //X
		read_uint8_t(peer, peer_getc); //Y
		read_int32_t(peer, peer_getc); //Z
		read_uint8_t(peer, peer_getc); //direction
		free_nbt_tag(read_nbt_tag(peer, peer_getc)); //held item
		read_uint8_t(peer, peer_getc); //cursor block x
		read_uint8_t(peer, peer_getc); //cursor block y
		read_uint8_t(peer, peer_getc); //cursor block z
		} break;
	case 0x10: { //held item change
		read_uint16_t(peer, peer_getc); //slot id
		} break;
	case 0x12: { //held item change
		read_int32_t(peer, peer_getc); //eid
		read_uint8_t(peer, peer_getc); //animation
		} break;
	case 0x13: { //entity action
		read_int32_t(peer, peer_getc); //eid
		read_uint8_t(peer, peer_getc); //action
		read_int32_t(peer, peer_getc); //jump boost
		} break;
	case 0x1b: {
		read_float(peer, peer_getc); //sideways
		read_float(peer, peer_getc); //forward
		read_uint8_t(peer, peer_getc); //jump
		read_uint8_t(peer, peer_getc); //unmount
		} break;
	case 0x65: { //close window
		read_uint8_t(peer, peer_getc); //window id
		} break;
	case 0x66: { // click window
		read_uint8_t(peer, peer_getc); //window id
		read_uint16_t(peer, peer_getc); //slot id
		read_uint8_t(peer, peer_getc); //button
		read_uint16_t(peer, peer_getc); //action number
		read_uint8_t(peer, peer_getc); //mode
		free_nbt_tag(read_nbt_tag(peer, peer_getc)); //clicked item
		} break;
	case 0x6a: { //confirm transaction
		read_uint8_t(peer, peer_getc); //window id
		read_uint16_t(peer, peer_getc); //action number
		read_uint8_t(peer, peer_getc); //accpeted
		} break;
	case 0x6b: { //creative inventory action
		read_uint16_t(peer, peer_getc); //action number
		free_nbt_tag(read_nbt_tag(peer, peer_getc)); //clicked item
		} break;
	case 0x6c: { //enchant item
		read_uint8_t(peer, peer_getc); //window id
		read_uint8_t(peer, peer_getc); //enchantment
		} break;
	case 0x82: { //update sign
		read_int32_t(peer, peer_getc); //x
		read_int16_t(peer, peer_getc); //y
		read_int32_t(peer, peer_getc); //z
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //line 1
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //line 2
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //line 3
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); //line 4
		} break;
	case 0xca: { //player abilities
		read_uint8_t(peer, peer_getc); // flags
		read_float(peer, peer_getc); // flying speed
		read_float(peer, peer_getc); // walking speed
		} break;
	case 0xcb: { //tab complete
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // text
		} break;
	case 0xcc: { //client settings
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // locale
		read_uint8_t(peer, peer_getc); // view distance
		read_uint8_t(peer, peer_getc); // chat flags
		read_uint8_t(peer, peer_getc); // difficulty
		read_uint8_t(peer, peer_getc); // show cape
		} break;
	case 0xcd: { //client statuses 
		read_uint8_t(peer, peer_getc); // payload
		} break;
	case 0xfa: { //plugin message
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // channel
		free_mc_string(read_nbt_string(peer, peer_getc)); // data
		} break;
	case 0xfc: { // encryption key response
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // shared secret
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // verify token response
		} break;
	case 0xfe: { //	server list ping
		read_uint8_t(peer, peer_getc); //magic byte
		} break;
	case 0xff: { //disconnect/kick
		free_mc_utf16(read_mc_utf16(peer, peer_getc)); // reason
		ntwk_disconnect_peer(conf, peer);
		} break;
	}
}

void disconnect78(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason){
	struct mc_utf16 str = mc_astr_to_utf16((struct mc_string){.str = reason, .len = strlen(reason)});
	write_disconnect(peer, peer_putchar, str);
	free_mc_utf16(str);
	ntwk_disconnect_peer(conf, peer);
}

void teleport78(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);
	write_teleport(peer, peer_putchar, cli->uuid.low, x, y, z, 0, 0); //TODO eid
}

void set_block78(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block){
	write_set_block(peer, peer_putchar, x, y, z, block);
}

void keep_alive78(struct ntwk_peer *peer){
	write_keep_alive(peer, peer_putchar, rand()); //TODO verify
}

void set_chunk78(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z){
	//TODO
	//write_chunk_data(peer, peer_putchar, cb, ca, x, z);
}
