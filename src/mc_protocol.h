#ifndef MC_PROTOCOL_HEADER_GUARD
#define MC_PROTOCOL_HEADER_GUARD

#include"stdint.h"
#include"stddef.h"
#include"ntwk.h"
#include"mc_dt.h"

struct mc_player {
	size_t protocol;

	double x, y, z;
	float pitch, yaw;
	struct mc_string name;
	struct mc_uuid uuid;

	size_t state;
};

struct mc_protocol {
	uint8_t valid;

	void (*handle_events)(struct ntwk_conf *conf, struct ntwk_peer *peer);

	void (*disconnect)(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason);
	void (*teleport)(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z);
	void (*set_block)(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block);
	void (*keep_alive)(struct ntwk_peer *peer);
	void (*set_chunk)(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *a, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z);
};


//TODO make this less terrible
void protocol_init(struct ntwk_conf *conf, struct ntwk_peer *peer);
void protocol_deinit(struct ntwk_conf *conf, struct ntwk_peer *peer);

void protocol_handle_events(struct ntwk_conf *conf, struct ntwk_peer *peer);
void protocol_disconnect(struct ntwk_conf *conf, struct ntwk_peer *peer, char *reason);

void protocol_teleport(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z);
void protocol_set_block(struct ntwk_peer *peer, int32_t x, int32_t y, int32_t z, int32_t block);
void protocol_keep_alive(struct ntwk_peer *peer);
void protocol_set_chunk(struct ntwk_peer *peer, void *cb, int32_t (*ca)(void *a, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z);

#endif
