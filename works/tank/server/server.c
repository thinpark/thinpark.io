/*
 * server.c
 *
 * Copyright (c) Zhou Peng <lockrecv@qq.com>
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "server.h"
#include "logger.h"
#include "map.h"
#include "link.h"
#include "cJSON.h"
#include "list.h"

/*
 * server command line arguments
 */
static const char *server_port    = "6000";
static const char *server_map     = "map.txt";
static const char *server_timeout = "400";
static const char *server_log     = "srv.log";

static struct option server_options[] = {
	{"port",    required_argument, 0, 'p'},
	{"map",     required_argument, 0, 'm'},
	{"timeout", required_argument, 0, 't'},
	{"stdout",  no_argument,       0, 's'},
	{0,         0,                 0,   0}
};

/*
 * this game
 */
static struct game game;

static void game_start(void)
{
	cJSON *root, *body;
	char *msg;
	size_t len;
	short tid;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "head", cJSON_CreateString("game start"));
	cJSON_AddItemToObject(root, "body", body=cJSON_CreateObject());
	msg = cJSON_Print(root);
	len = strlen(msg);

	for (tid = 0; tid < TEAM_MAX; tid++)
		write(game.teams[tid].sockfd, msg, len);

	cJSON_Delete(root);
	free(msg);
}

static void game_over(void)
{
	cJSON *root, *body;
	char *msg;
	size_t len;
	short tid;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "head", cJSON_CreateString("game over"));
	cJSON_AddItemToObject(root, "body", body=cJSON_CreateObject());
	msg = cJSON_Print(root);
	len = strlen(msg);

	for (tid = 0; tid < TEAM_MAX; tid++)
		write(game.teams[tid].sockfd, msg, len);

	cJSON_Delete(root);
	free(msg);
}

static void leg_start(void)
{
	cJSON *root, *body;
	char *msg;
	size_t len;
	short tid;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "head", cJSON_CreateString("leg start"));
	cJSON_AddItemToObject(root, "body", body=cJSON_CreateObject());
	msg = cJSON_Print(root);
	len = strlen(msg);

	for (tid = 0; tid < TEAM_MAX; tid++)
		write(game.teams[tid].sockfd, msg, len);

	cJSON_Delete(root);
	free(msg);
}

static void leg_end(void)
{
	cJSON *root, *body;
	char *msg;
	size_t len;
	short tid;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "head", cJSON_CreateString("leg end"));
	cJSON_AddItemToObject(root, "body", body=cJSON_CreateObject());
	msg = cJSON_Print(root);
	len = strlen(msg);

	for (tid = 0; tid < TEAM_MAX; tid++)
		write(game.teams[tid].sockfd, msg, len);

	cJSON_Delete(root);
	free(msg);
}

static int round_step(void)
{
	logger_info("%s\n", "round step");
	return 1;
}

int main(int argc, char *argv[])
{
	/* server options */
	int c;
	while ((c = getopt_long(argc, argv, "p:m:t:s", server_options, NULL)) != -1) {
		switch (c) {
		case 'p':
			server_port = optarg;
			break;
		case 'm':
			server_map = optarg;
			break;
		case 't':
			server_timeout = optarg;
			break;
		case 's':
			server_log = "/dev/stdout";
			break;
		default:
			exit(-1);
		}
	}

	/* logger setup */
	if (!logger_open(server_log))
		exit(-2);

	/* load map */
	if (!map_load(server_map))
		exit(-3);

	/* net link open */
	if (!link_open(server_port))
		exit(-4);

	/* game initialization */
	game.leg_remain = LEG_MAX;
	game.round_remain = ROUND_MAX;
	short tm, tk;
	for (tm = 0; tm < TEAM_MAX; tm++) {
		game.teams[tm].id = tm;
		game.teams[tm].life_remain = LIFE_MAX;
		game.teams[tm].sockfd = link_accept();
		INIT_LIST_HEAD(&game.teams[tm].stars);
		INIT_LIST_HEAD(&game.teams[tm].bullets);

		for (tk = 0; tk < TANK_MAX; tk++) {
			game.teams[tm].tanks[tk].id = tm * TANK_MAX + tk;
			game.teams[tm].tanks[tk].star_count = 0;

			/* alloc tank postion */
			short y, x;
			for (y = 0; y < Y_MAX; y++) {
				for (x = 0; x < X_MAX; x++) {
					if (map_get(y, x) == AREA) {
						game.teams[tm].tanks[tk].pos.y = y;
						game.teams[tm].tanks[tk].pos.x = x;
						map_set(y, x, TANK);
						goto tank_ok;
					}
				}
			}
			logger_warn("%s\n", "map area less than tanks.");
			exit(-4);
		tank_ok:
			logger_info("team: %hi, tank: %hi, at position (%hi, %hi)\n",
				    game.teams[tm].id,
				    game.teams[tm].tanks[tk].id,
				    game.teams[tm].tanks[tk].pos.y,
				    game.teams[tm].tanks[tk].pos.x);
		}
	}

	/* play game */
	game_start();
	while (game.leg_remain-- > 0) {
		leg_start();
		while (game.round_remain-- > 0 && round_step());
		game.round_remain = ROUND_MAX;
		leg_end();
	}
	game_over();

	link_close();
	logger_close();

	return 0;
}
