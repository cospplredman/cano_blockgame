//https://wiki.vg/index.php?title=Protocol&oldid=4899#Login_Request_.280x01.29

WRITE_ALL(keep_alive, (int32_t id),
	(uint8_t, 0x00)
	(int32_t, id)
)

WRITE_ALL(login_request, (),
	(uint8_t, 0x01)
	(int32_t, 0) //eid
	(mc_utf16, ((struct mc_utf16){.str = u"default", .len = 7, .code_points = 7})) // level type
	(uint8_t, 1) //gamemode
	(uint8_t, 0) // Dimension
	(uint8_t, 0) //difficulty
	(uint8_t, 0)
	(uint8_t, 255) //max players
)

WRITE_ALL(spawn_position, (int32_t x, int32_t y, int32_t z),
	(uint8_t, 0x06)
	(int32_t, x)
	(int32_t, y)
	(int32_t, z)
)

WRITE_ALL(pos_look, (double x, double y, double z, float pitch, float yaw),
	(uint8_t, 0x0d)
	(double, x)
	(double, (y+1)) // stance
	(double, y)
	(double, z)
	(float, yaw)
	(float, pitch)
	(uint8_t, 0) //on ground
)

WRITE_ALL(teleport, (int32_t eid, int32_t x, int32_t y, int32_t z, float pitch, float yaw),
	(uint8_t, 0x22)
	(int32_t, x)
	(int32_t, y)
	(int32_t, z)
	(float, yaw)
	(float, pitch)
)

WRITE_ALL(disconnect, (struct mc_utf16 reason),
	(uint8_t, 0xff)
	(mc_utf16, reason)
)

WRITE_ALL(chunk_data_header, (int32_t x, int32_t z),
	(uint8_t, 0x33)
	(int32_t, x)
	(int32_t, z)
	(uint8_t, 1) //ground up continuous
	(uint16_t, 0xffff) //included sub chunks
	(uint16_t, 0xffff) //included additional block data
)

WRITE_ALL(set_block, (int32_t x, int32_t y, int32_t z, int32_t block),
	(uint8_t, 0x35)
	(int32_t, x)
	(uint8_t, y)
	(int32_t, z)
	(int16_t, block)
	(uint8_t, 0) //block metadata
)

