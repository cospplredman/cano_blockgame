WRITE_ALL(status_request, (),
	(VarInt, 0x0)
)

WRITE_ALL(handshake, (int32_t protocol, struct mc_string addr, uint16_t port, int32_t next),
	(VarInt, 0x0)
	(VarInt, protocol)
	(mc_string, addr)
	(uint16_t, port)
	(VarInt, next)
)

WRITE_ALL(status_response, (struct mc_string motd), 
	(VarInt, 0x0)
	(mc_string, motd)
) 

WRITE_ALL(ping_response, (int64_t pv),
	(VarInt, 0x1)
	(int64_t, pv)
)

WRITE_ALL(disconnect, (struct mc_string msg), 
	(VarInt, 0x00)
	(mc_string, msg)
)

WRITE_ALL(login_start, (struct mc_string username),
	(VarInt, 0x00)
	(mc_string, username)
	(uint8_t, 0)
)

WRITE_ALL(login_success, (struct mc_string username, struct mc_uuid uuid),
	(VarInt, 0x02)
	(mc_uuid, uuid)
	(mc_string, username)
	(VarInt, 0x0)
)

WRITE_ALL(login, (int32_t eid, struct nbttag registry),
	(VarInt, 0x28)
	(int32_t, eid)
	(uint8_t, 0)
	(uint8_t, 3)
	(int8_t, 0)
	(VarInt, 1)
	(mc_string, (MC_STR("minecraft:overworld")))

	(nbt_tag, registry)

	(mc_string, (MC_STR("minecraft:overworld")))
	(mc_string, (MC_STR("minecraft:overworld")))
	(int64_t, 0)
	(VarInt, 10)
	(VarInt, 2)
	(VarInt, 2)
	(uint8_t, 0)
	(uint8_t, 1)
	(uint8_t, 0)
	(uint8_t, 0)
	(uint8_t, 0)
)


WRITE_ALL(syncronize_player_position, (double x, double y, double z, float pitch, float yaw),
	(VarInt, 0x3c)
	(double, x)
	(double, y)
	(double, z)
	(float, pitch)
	(float, yaw)
	(uint8_t, 0)
	(VarInt, 69) //TODO pass this
)


WRITE_ALL(player_info_update, (struct mc_string username, struct mc_uuid uuid),
	(VarInt, 0x3A)
	(uint8_t, 3)
	(VarInt, 1)
	(mc_uuid, uuid)
	(mc_string, username)
	(VarInt, 0)
	(uint8_t, 0)
)

WRITE_ALL(set_default_spawn_position, (int x, int y, int z, float angle),
	(VarInt, 0x50)
	(uint64_t, ((uint64_t)x << 38) + ((uint64_t)z << 12) + y)
	(float, angle)
)

WRITE_ALL(bundle_delimiter, (struct ntwk_peer *peer), 
	(VarInt, 0x0)
)

WRITE_ALL(spawn_player, (int32_t eid, struct mc_uuid uuid, double x, double y, double z, float pitch, float yaw),
	(VarInt, 0x3)
	(VarInt, eid)
	(mc_uuid, uuid)
	(double, x)
	(double, y)
	(double, z)
	(uint8_t, pitch / M_PI * 128)
	(uint8_t, yaw / M_PI * 128)
)

WRITE_ALL(keep_alive, (int64_t ch),
	(VarInt, 0x23)
	(int64_t, ch)
)

WRITE_ALL(chunk_data, (void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t x, int32_t z),
	(VarInt, 0x24)
	(int32_t, x)
	(int32_t, z)

	(nbt_tag, NBT_TAG_COMPOUND("", 0)) //TODO heightmap
	(mc_chunk, cb, ca, x, z)

	(VarInt, 0)
	(uint8_t, 0)
	(mc_bitset, ((struct mc_bitset){0, NULL}))
	(mc_bitset, ((struct mc_bitset){0, NULL}))
	(mc_bitset, ((struct mc_bitset){0, NULL}))
	(mc_bitset, ((struct mc_bitset){0, NULL}))
	(VarInt, 0)
	(VarInt, 0)
)

WRITE_ALL(set_render_dist, (int32_t dist),
	(VarInt, 0x4F)
	(VarInt, dist)
)

WRITE_ALL(center_chunk, (int32_t x, int32_t z),
	(VarInt, 0x4e)
	(VarInt, x)
	(VarInt, z)
)
