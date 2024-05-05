NBT_TAG_COMPOUND("", 4,
	NBT_TAG_COMPOUND("minecraft:chat_type", 2,
		NBT_TAG_STRING("type", "minecraft:chat_type"),
		NBT_TAG_LIST("value", NBT_TAG_COMPOUND, 7,
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:chat"),
				NBT_TAG_INT("id", 0),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:emote_command"),
				NBT_TAG_INT("id", 1),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 2,
						NBT_TAG_STRING("translation_key", "chat.type.emote"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.emote"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:msg_command_incoming"),
				NBT_TAG_INT("id", 2),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 3,
						NBT_TAG_STRING("translation_key", "commands.message.display.incoming"),
						NBT_TAG_COMPOUND("style", 2,
							NBT_TAG_STRING("color", "gray"),
							NBT_TAG_BYTE("italic", 1)
						),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:msg_command_outgoing"),
				NBT_TAG_INT("id", 3),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 3,
						NBT_TAG_STRING("translation_key", "commands.message.display.outgoing"),
						NBT_TAG_COMPOUND("style", 2,
							NBT_TAG_STRING("color", "gray"),
							NBT_TAG_BYTE("italic", 1)
						),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "target"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:say_command"),
				NBT_TAG_INT("id", 4),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 2,
						NBT_TAG_STRING("translation_key", "chat.type.announcement"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:team_msg_command_incoming"),
				NBT_TAG_INT("id", 5),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 2,
						NBT_TAG_STRING("translation_key", "chat.type.team.text"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 3,
							NBT_TAG_STRING(NULL, "target"),
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:team_msg_command_outgoing"),
				NBT_TAG_INT("id", 6),
				NBT_TAG_COMPOUND("element", 2,
					NBT_TAG_COMPOUND("chat", 2,
						NBT_TAG_STRING("translation_key", "chat.type.team.sent"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 3,
							NBT_TAG_STRING(NULL, "target"),
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					),
					NBT_TAG_COMPOUND("narration", 2,
						NBT_TAG_STRING("translation_key", "chat.type.text.narrate"),
						NBT_TAG_LIST("parameters", NBT_TAG_STRING, 2,
							NBT_TAG_STRING(NULL, "sender"),
							NBT_TAG_STRING(NULL, "content")
						)
					)
				)
			)
		)
	),
	NBT_TAG_COMPOUND("minecraft:dimension_type", 2,
		NBT_TAG_STRING("type", "minecraft:dimension_type"),
		NBT_TAG_LIST("value", NBT_TAG_COMPOUND, 1,
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:overworld"),
				NBT_TAG_INT("id", 0),
				NBT_TAG_COMPOUND("element", 17,
					NBT_TAG_BYTE("piglin_safe", 0),
					NBT_TAG_BYTE("natural", 1),
					NBT_TAG_FLOAT("ambient_light", 0x1p-127),
					NBT_TAG_INT("monster_spawn_block_light_limit", 0),
					NBT_TAG_STRING("infiniburn", "#minecraft:infiniburn_overworld"),
					NBT_TAG_BYTE("respawn_anchor_works", 0),
					NBT_TAG_BYTE("has_skylight", 1),
					NBT_TAG_BYTE("bed_works", 1),
					NBT_TAG_STRING("effects", "minecraft:overworld"),
					NBT_TAG_BYTE("has_raids", 1),
					NBT_TAG_INT("logical_height", 256),
					NBT_TAG_DOUBLE("coordinate_scale", 0x1p+0),
					NBT_TAG_COMPOUND("monster_spawn_light_level", 2,
						NBT_TAG_STRING("type", "minecraft:uniform"),
						NBT_TAG_COMPOUND("value", 2,
							NBT_TAG_INT("min_inclusive", 0),
							NBT_TAG_INT("max_inclusive", 7)
						)
					),
					NBT_TAG_INT("min_y", 0),
					NBT_TAG_BYTE("ultrawarm", 0),
					NBT_TAG_BYTE("has_ceiling", 0),
					NBT_TAG_INT("height", 256)
				)
			)
		)
	),
	NBT_TAG_COMPOUND("minecraft:damage_type", 2,
		NBT_TAG_STRING("type", "minecraft:damage_type"),
		NBT_TAG_LIST("value", NBT_TAG_COMPOUND, 42,
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:arrow"),
				NBT_TAG_INT("id", 0),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "arrow")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:bad_respawn_point"),
				NBT_TAG_INT("id", 1),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("scaling", "always"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "badRespawnPoint"),
					NBT_TAG_STRING("death_message_type", "intentional_game_design")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:cactus"),
				NBT_TAG_INT("id", 2),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "cactus")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:cramming"),
				NBT_TAG_INT("id", 3),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "cramming")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:dragon_breath"),
				NBT_TAG_INT("id", 4),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "dragonBreath")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:drown"),
				NBT_TAG_INT("id", 5),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "drowning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "drown")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:dry_out"),
				NBT_TAG_INT("id", 6),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "dryout")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:explosion"),
				NBT_TAG_INT("id", 7),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "always"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "explosion")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:fall"),
				NBT_TAG_INT("id", 8),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "fall"),
					NBT_TAG_STRING("death_message_type", "fall_variants")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:falling_anvil"),
				NBT_TAG_INT("id", 9),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "anvil")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:falling_block"),
				NBT_TAG_INT("id", 10),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "fallingBlock")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:falling_stalactite"),
				NBT_TAG_INT("id", 11),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "fallingStalactite")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:fireball"),
				NBT_TAG_INT("id", 12),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "fireball")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:fireworks"),
				NBT_TAG_INT("id", 13),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "fireworks")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:fly_into_wall"),
				NBT_TAG_INT("id", 14),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "flyIntoWall")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:freeze"),
				NBT_TAG_INT("id", 15),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "freezing"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "freeze")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:generic"),
				NBT_TAG_INT("id", 16),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "generic")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:hot_floor"),
				NBT_TAG_INT("id", 17),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "hotFloor")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:in_fire"),
				NBT_TAG_INT("id", 18),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "inFire")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:in_wall"),
				NBT_TAG_INT("id", 19),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "inWall")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:indirect_magic"),
				NBT_TAG_INT("id", 20),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "indirectMagic")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:lava"),
				NBT_TAG_INT("id", 21),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "lava")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:lightning_bolt"),
				NBT_TAG_INT("id", 22),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "lightningBolt")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:magic"),
				NBT_TAG_INT("id", 23),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "magic")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:mob_attack"),
				NBT_TAG_INT("id", 24),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "mob")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:mob_attack_no_aggro"),
				NBT_TAG_INT("id", 25),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "mob")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:mob_projectile"),
				NBT_TAG_INT("id", 26),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "mob")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:on_fire"),
				NBT_TAG_INT("id", 27),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "onFire")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:out_of_world"),
				NBT_TAG_INT("id", 28),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "outOfWorld")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:player_attack"),
				NBT_TAG_INT("id", 29),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "player")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:player_explosion"),
				NBT_TAG_INT("id", 30),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "always"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "explosion.player")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:sonic_boom"),
				NBT_TAG_INT("id", 31),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "always"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "sonic_boom")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:stalagmite"),
				NBT_TAG_INT("id", 32),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "stalagmite")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:starve"),
				NBT_TAG_INT("id", 33),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "starve")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:sting"),
				NBT_TAG_INT("id", 34),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "sting")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:sweet_berry_bush"),
				NBT_TAG_INT("id", 35),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "poking"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "sweetBerryBush")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:thorns"),
				NBT_TAG_INT("id", 36),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "thorns"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "thorns")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:thrown"),
				NBT_TAG_INT("id", 37),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "thrown")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:trident"),
				NBT_TAG_INT("id", 38),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "trident")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:unattributed_fireball"),
				NBT_TAG_INT("id", 39),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_STRING("effects", "burning"),
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "onFire")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:wither"),
				NBT_TAG_INT("id", 40),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1p-127),
					NBT_TAG_STRING("message_id", "wither")
				)
			),
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:wither_skull"),
				NBT_TAG_INT("id", 41),
				NBT_TAG_COMPOUND("element", 3,
					NBT_TAG_STRING("scaling", "when_caused_by_living_non_player"),
					NBT_TAG_FLOAT("exhaustion", 0x1.99999ap-4),
					NBT_TAG_STRING("message_id", "witherSkull")
				)
			)
		)
	),
	NBT_TAG_COMPOUND("minecraft:worldgen/biome", 2,
		NBT_TAG_STRING("type", "minecraft:worldgen/biome"),
		NBT_TAG_LIST("value", NBT_TAG_COMPOUND, 1,
			NBT_TAG_COMPOUND(NULL, 3,
				NBT_TAG_STRING("name", "minecraft:plains"),
				NBT_TAG_INT("id", 0),
				NBT_TAG_COMPOUND("element", 4,
					NBT_TAG_COMPOUND("effects", 5,
						NBT_TAG_INT("sky_color", 7907327),
						NBT_TAG_INT("water_fog_color", 329011),
						NBT_TAG_INT("fog_color", 12638463),
						NBT_TAG_INT("water_color", 4159204),
						NBT_TAG_COMPOUND("mood_sound", 4,
							NBT_TAG_INT("tick_delay", 6000),
							NBT_TAG_DOUBLE("offset", 0x1p+1),
							NBT_TAG_STRING("sound", "minecraft:ambient.cave"),
							NBT_TAG_INT("block_search_extent", 8)
						)
					),
					NBT_TAG_BYTE("has_precipitation", 1),
					NBT_TAG_FLOAT("temperature", 0x1.99999ap-1),
					NBT_TAG_FLOAT("downfall", 0x1.99999ap-2)
				)
			)
		)
	)
)
