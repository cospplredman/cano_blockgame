#include"mc_protocol.h"

void protocol_init(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player **cli = (struct mc_player**)ntwk_peer_get_data(peer);
	*cli = malloc(sizeof(struct mc_player));
	**cli = (struct mc_player){
		.protocol = 0,
		.state = 0,
		.name = MC_STR(NULL)
		//TODO init
	};
}

void protocol_deinit(struct ntwk_conf *conf, struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);
	free_mc_string(cli->name);
	free(cli);
}

#define PROTOCOL(num) \
void handle_events##num(struct ntwk_conf *conf, struct ntwk_peer *peer); \
void disconnect##num(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason); \
void teleport##num(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z); \
void set_block##num(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block); \
void keep_alive##num(struct ntwk_peer *peer); \
void set_chunk##num(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *a, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z);

#include"mc_protocol.inc"
#undef PROTOCOL

struct mc_protocol protocol_vtable[] = {
#define PROTOCOL(num) [num] = (struct mc_protocol){\
       	.valid = 1, \
	.handle_events = handle_events##num, \
	.disconnect = disconnect##num, \
	.teleport = teleport##num, \
	.set_block = set_block##num, \
	.keep_alive = keep_alive##num, \
	.set_chunk = set_chunk##num, \
},
#include"mc_protocol.inc"
};

static int is_valid_protocol(size_t proto){
	return proto < sizeof(protocol_vtable)/sizeof(struct mc_protocol) && protocol_vtable[proto].valid;
}

#define GET_PROTO struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer); \
	if(is_valid_protocol(cli->protocol))protocol_vtable[cli->protocol]

void protocol_handle_events(struct ntwk_conf *conf, struct ntwk_peer *peer){
	GET_PROTO.handle_events(conf, peer);
}

void protocol_disconnect(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason){
	GET_PROTO.disconnect(conf, peer, reason);	
}

void protocol_teleport(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z){
	GET_PROTO.teleport(peer, x, y, z);
}

void protocol_set_block(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block){
	GET_PROTO.set_block(peer, x, y, z, block);
}

void protocol_keep_alive(struct ntwk_peer *peer){
	GET_PROTO.keep_alive(peer);
}


void protocol_set_chunk(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *a, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z){
	GET_PROTO.set_chunk(peer, cb, ca, x, z);
}


