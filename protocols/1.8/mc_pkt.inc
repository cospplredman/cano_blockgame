
WRITE_ALL(keep_alive, (int64_t ch),
	(VarInt, 0x0)
	(VarInt, ch)
)

WRITE_ALL(status_response, (struct mc_string motd), 
	(VarInt, 0x0)
	(mc_string, motd)
) 

WRITE_ALL(ping_response, (int64_t pv),
	(VarInt, 0x1)
	(int64_t, pv)
)

WRITE_ALL(login_success, (struct mc_string username, struct mc_string uuid),
	(VarInt, 0x02)
	(mc_string, uuid)
	(mc_string, username)
)

WRITE_ALL(join_game, (int32_t eid),
	(VarInt, 0x01)
	(int32_t, eid)
	(uint8_t, 1)
	(int8_t, 0)
	(uint8_t, 0)
	(uint8_t, 255)
	(mc_string, (MC_STR("default")))
	(uint8_t, 0)
)


WRITE_ALL(set_default_spawn_position, (int x, int y, int z),
	(VarInt, 0x05)
	(uint64_t, ((uint64_t)x << 38) + ((uint64_t)z << 12) + y)
)

WRITE_ALL(pos_look, (double x, double y, double z, float pitch, float yaw),
	(VarInt, 0x08)
	(double, x)
	(double, y)
	(double, z)
	(float, yaw)
	(float, pitch)
	(uint8_t, 0)
)

WRITE_ALL(disconnect, (struct mc_string reason),
	(VarInt, 0x40)
	(mc_string, reason)
)
