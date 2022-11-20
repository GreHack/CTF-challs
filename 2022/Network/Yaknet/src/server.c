// C program for the Server Side

// inet_addr
#include <arpa/inet.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#include "util.h"
#include "proto.h"
#include "qoi.h"
#include "math.h"
/*

  0. Baby demo of the chall just scrall the map (en reseau avec tout le monde ?)
  1. premier tp dans les murs/obstacle (gdb mode debug), passe a travers un obstacle
  2. longue ligne droite (obligé de scripter proxy/gdbscript), ajouter pour  tp possible

  3. maps mega grande (petit exploit pour discover the map la command GET verifie pas si le joueur est pas loin) +  TP NON POSSINLE( + script oglibatoire) (hint from the binary? but it is not a reverse bin challenge) ideas :
        -->  commande help ?

  4. impossible LEVEL blind map (no get no tp) , juste super algo pour tout parcourir la map

*/
int LEVEL = 0;
int PORT = 12345;
int NO_TP = 0;
int NO_WALK_HACK = 0;
int NO_FAR_VISION = 0;

static float
vec2_dist(vec2 a, vec2 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	return sqrt(x*x + y*y);
}

char *map;
size_t map_size_x = 0;
size_t map_size_y = 0;
vec2 map_start;
vec2 map_flag;

char *flags[] = {
	"\2GH22{L4B1RY4TH3_h3ll0_W0rld}",
	"\2GH22{L4B1RY4TH3_pr0_w4llH4Ck}",
	"\2GH22{L4B1RY4TH3_n0CliP_Bl1nD}",
	"\2now you have to draw this map", // GH22{L4B1RY4TH3_az7ec_4bducti0n}
	"\2GH22{L4B1RY4TH3_the_ne7rUnner}",
};

static int map_get(int x, int y)
{
	return map[x * map_size_y + y];
}


struct Player {
	char name[32];
	vec2 position;
};

struct Player player1 = {
	.name = "player1",
	.position = { .x = 1, .y = 0 },
};

bool map_can_show(struct Player p, int x, int y) {
    // check if in the map
	if (!(x < (int)map_size_x && y < (int)map_size_y && 0 <= x && 0 <= y )) {
        return false;
    }
    // check if close to the player (for level 4)
    if( NO_FAR_VISION ) {
        if ( !( p.position.x - 15 <= x && x <= p.position.x + 15 )) {
            return false;
        }
        if ( !( p.position.y - 15 <= y && y <= p.position.y + 15 )) {
            return false;
        }
    }

    return true;

}

static void player_info(struct Player player)
{
	printf("player name:%s , position.x %f .y %f\n", player.name, player.position.x, player.position.y);
}

int move_player(struct Player * player, float x, float y)
{
	vec2 p = {(int)(x + 0.5), (int)(y + 0.5)};

	//printf("trying x:%f , y:%f\n", x, y );
    if( p.x  >= map_size_x || p.x < 0  ) {
        printf("can't move to this x\n");
        return 0;
    }

    if( p.y >= map_size_y || p.y < 0 ) {
        printf("can't move to this y\n");
        return 0;
    }

    if (NO_WALK_HACK) {
	    int type = map_get(p.x, p.y);
	    if (type == TYPE_WALL || type == TYPE_HOLE) {
	    float ex = type == TYPE_HOLE ? 0.3 : 0.5;
	    /* quad centered in p */
	    if (((p.x - ex) < x && x < (p.x + ex)) &&
		((p.y - ex) < y && y < (p.y + ex))) {
		    printf("cannot move inside wall\n");
		    return 0;
	    }
	    }
    }
    // player can't tp for level 3-4
    if (NO_TP) {
	    vec2 np = {x,y};
	    if (vec2_dist(player->position, np) > 0.3) {
		    printf("cannot move too far\n");
		    return 0;
	    }
    }

    player->position.x = x;
    player->position.y = y;

	return 1;
}
/*
  le serveur à un labyrinthe en mémoire et potentiellemnt la position du joueur
  le joueur envois des messages au serveur du genre
  - "visible @", le srv reponds une list de coord autour du joueur
  - "get @" pour savoir si c'est un mur ou un sol à une certaine coord
  ouai
  je vois
  - et un autre pour mettre a jour la position du joueur dans le serveur
  - activé =! check,
  - cmd flag

*/

void* client_manager(void* sock)
{
	int socket = *((int *)sock);

	/* send start position */
	send_pos(socket, map_start.x, map_start.y);
	printf("start @ %f %f\n",  map_start.x, map_start.y);
	player1.position.x = map_start.x;
	player1.position.y = map_start.y;

	while (1) {
		struct cmd_msg m = { 0 };
		ssize_t n;
		n = recv_cmd(socket, &m);
		if (n < 0)
			goto exit;
		if (n != sizeof(m)) {
			fprintf(stderr, "recv invalid len %zd should be = %zd\n", n, sizeof(m));
			goto exit;
		}
		//printf("recv cmd %d '%s'\n", m.cmd, cmd_name(m.cmd));
		switch (m.cmd) {
		default:
			fprintf(stderr, "unknown cmd, exitting\n");
			fallthrough;
		case ERR:
			goto exit;
			break;
		case RSP:
			break;
		case HEY:
			printf("hey new player: %s\n", m.hey.name);
			strncpy(player1.name, m.hey.name, sizeof(player1.name) - 1);
			player1.name[sizeof(player1.name) - 1] = '\0';
			process_hey(socket, m.hey.name);
			break;
        case GET:
            //printf("asked x : %d , y : %d\n", m.get.x, m.get.y);
            // check if not outside the map or too far for the user
            if (map_can_show(player1, m.get.x, m.get.y )) {
		        process_get(socket, m.get.x, m.get.y, map_get(m.get.x, m.get.y));
            } else {
		        process_get(socket, m.get.x, m.get.y, -1);
                //send_rsp(socket, ERR, "Can not show you that");
            }
			break;
		case MOV:
		    //player_info(player1);
		    if( ! move_player(&player1, m.mov.x, m.mov.y)) {
		        //player_info(player1);
                send_pos(socket, player1.position.x, player1.position.y );
		    }
		    if (vec2_dist(player1.position, map_flag) < 0.45) {
		        player_info(player1);
		        printf("well done player %s validate the level %d sending him the flag\n", player1.name, LEVEL);
		        send_rsp(socket, RSP, flags[LEVEL] );
		    }
		    break;
		case BYE:
			printf("bye bye player\n");
			goto exit;
			break;
		case VIS:
			break;

		}
	}
exit:
	fprintf(stderr, "connection closed: %s\n", strerror(errno));
	close(socket);
	return 0;
}

static void
load_map(char *file)
{
	unsigned char *data;
	qoi_desc desc;
	size_t i;
	data = qoi_read(file, &desc, 3);
	if (!data) {
		fprintf(stderr, "error loading %s: %s\n", file, strerror(errno));
		exit(1);
	}
	map_size_x = desc.width;
	map_size_y = desc.height;
	map = malloc(map_size_x * map_size_y * sizeof(char));
	for (i = 0; i < map_size_x * map_size_y; i++) {
		uint32_t rgb = data[3 * i + 0] << 16 |
			(data[3 * i + 1] << 8) |
			(data[3 * i + 2] << 0);
		if (rgb == 0xffffff) /* wall */
			map[i] = TYPE_WALL;
		else if (rgb == 0xff0000) /* hole Red */
			map[i] = TYPE_HOLE;
		else if (rgb == 0x00ff00) { /* start Green */
			map[i] = TYPE_TILE; /* walkable */
			map_start.x = i / map_size_y;
			map_start.y = i % map_size_y;
		}
		else if (rgb == 0x0000ff) { /* flag Blue */
			map[i] = TYPE_FLAG;
			map_flag.x = i / map_size_y;
			map_flag.y = i % map_size_y;
		} else
			map[i] = TYPE_TILE; /* walkable */
	}
	free(data);
}

// Driver Code
int main(int argc,char* argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	pid_t pid_fork;
	int opt = 1;
	char peer_addr[INET6_ADDRSTRLEN + 1];

	if (argc == 3) {
		PORT = atoi(argv[1]);
		LEVEL=atoi(argv[2]);
	} else {
	    printf("No port or level specify will use default ones\n");
	    printf("Usage: ./server [port] [level]\n");
	}

	printf("PORT : %d\n", PORT);
	printf("LEVEL : %d\n", LEVEL);

	if( LEVEL == 0) {
		load_map("map0.qoi");
	} else if (LEVEL == 1) {
		load_map("map1.qoi");
	} else if (LEVEL == 2) {
		load_map("map2.qoi");
		NO_WALK_HACK=true;
		NO_TP=true;
	} else if (LEVEL == 3) {
		NO_TP = true;
		NO_WALK_HACK=true;
		load_map("map3.qoi");
	} else if (LEVEL == 4) {
		NO_TP = true;
		NO_FAR_VISION = true;
		NO_WALK_HACK = true;
		load_map("map4.qoi");
	} else {
		fprintf(stderr, "no such level '%d'\n", LEVEL);
		exit(1);
	}

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
		       SO_REUSEADDR | SO_REUSEPORT, &opt,
		       sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&address,
		 sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Extract the first
		// connection in the queue
		if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
					 (socklen_t*)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		pid_fork = fork();
		if (pid_fork < 0) { /* error occurred */
			perror("Fork failed");
			shutdown(new_socket, SHUT_RDWR);
			close(new_socket);
			continue;
		}
		if (pid_fork == 0) { /* child process */
			client_manager(&new_socket);
		} else if (pid_fork > 0) { /* parent process */
			close(new_socket); /* close the socket */
		}
		// parent continue in the mail loop

	    /*if (pthread_create(&clientthread[i++], NULL, client_manager, &new_socket) != 0)
		{
			// Error in creating thread
			printf("Failed to create thread\n");
		}*/
	}
	printf("quitting the main loop and waiting for child\n");

	return 0;
}
