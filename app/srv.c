#include<stdio.h>
#include<unistd.h>
#include<stddef.h>
#include<assert.h>
#include<signal.h>
#include <sys/time.h>

#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include"mc_protocol.h"

// #include <time.h>
double get_time(){
        struct timeval start;
        gettimeofday(&start, NULL);
        //printf("%d %d\n", start.tv_sec, start.tv_usec);

        //TODO probably don't need this precision
        double us = (start.tv_sec*1e6 + start.tv_usec);
        return us * 1e-6;

//      return clock() / CLOCKS_PER_SEC
}


int peer_getc(void *p){
	return ntwk_getc((struct ntwk_peer*)p);
}

int peer_putchar(void *p, int v){
	return ntwk_putchar((struct ntwk_peer*)p, v);
}

int print_putchar(void *p, int val){
	return putchar(val);
}
static int flag_die = 0;
static int64_t eid = 0;
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





void sigint_handler(int signum) {
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigint_handler);
	signal(SIGKILL, sigint_handler);
	flag_die = 1;
}

int32_t world_fn(void *a, int32_t x, int32_t y, int32_t z){
	double rn = (double)rand() / (double)RAND_MAX;
	if(rn < 0.1)
		return 1;
	return 0;
}

void push_world_data(struct ntwk_peer *peer){
	struct mc_player *cli = *(struct mc_player**)ntwk_peer_get_data(peer);

	int32_t cx = (int32_t)(cli->x/16), cz = (int32_t)(cli->z/16);

	for(int32_t i = -5; i < 6; i++)
		for(int32_t j = -5; j < 6; j++)
			protocol_set_chunk(peer, NULL, world_fn, cx + i, cz + j);
}

int main(){
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigint_handler);
	signal(SIGKILL, sigint_handler);
	
	struct ntwk_conf *conf = ntwk_init(
		25565,
		256,
		protocol_handle_events,
		protocol_init,
		protocol_deinit
	);

	if(conf == NULL){
		perror("ntwk");
		return EXIT_FAILURE;
	}

	double time = get_time();
	double tick = time;
	while(1){
		ntwk_handle_events(conf);
		
		if(get_time() - time > 20){
			printf("sending keep alive:\n");
			size_t peers;
			struct ntwk_peer *peer = ntwk_get_peers(conf, &peers);
			for(int i = 0; i < peers; i++)
				protocol_keep_alive(peer + i);
			time = get_time();
		}

		if(get_time() - tick > 5){
			size_t peers;
			struct ntwk_peer *peer = ntwk_get_peers(conf, &peers);
			for(int i = 0; i < peers; i++)
				push_world_data(peer + i);
			tick = get_time();
		}

		if(flag_die)
			break;
	}

	printf("cleaning up\n");
	ntwk_deinit(conf);
}

