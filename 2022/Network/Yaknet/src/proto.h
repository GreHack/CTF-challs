#define LEN(a) (sizeof(a)/sizeof(*a))
#define fallthrough //[[fallthrough]]

enum cmd {
	ERR = 0,
	RSP,
	HEY,
	BYE,
	VIS,
	GET,
	IAT,
	MOV,
	POS,
};

const char *cmd_names[] = {
	[ERR] = "err",
	[RSP] = "rsp",
	[HEY] = "hey",
	[BYE] = "bye",
	[VIS] = "vis",
	[GET] = "get",
	[IAT] = "iat",
	[MOV] = "mov",
	[POS] = "pos",
};

static inline const char *cmd_name(unsigned int cmd) {
	if (cmd < LEN(cmd_names))
		return cmd_names[cmd] != NULL ? cmd_names[cmd] : "";
	return "invalid";
}

struct cmd_msg {
	uint8_t cmd;
	union {
		struct cmd_rsp {
			uint32_t val;
			char msg[32];
		} rsp;
		struct cmd_hey {
			char name[32];
		} hey;
		struct cmd_bye {
		} bye;
		struct cmd_get {
			uint16_t x;
			uint16_t y;
#define TYPE_NONE -1
#define TYPE_TILE 0 /* walkable */
#define TYPE_WALL 1
#define TYPE_HOLE 2
#define TYPE_FLAG 3
			int8_t val;
		} get;
		struct cmd_mov {
		    float x;
		    float y;
		} mov;
		struct cmd_pos {
		    float x;
		    float y;
		} pos;
	};
};

static inline int recv_rdy(int socket, struct cmd_msg *cmd) {
	return recv(socket, cmd, sizeof(*cmd), MSG_PEEK | MSG_DONTWAIT);
}

static inline ssize_t recv_cmd(int socket, struct cmd_msg *cmd) {
	return recv(socket, cmd, sizeof(*cmd), MSG_WAITALL);
}

static inline ssize_t send_cmd(int socket, struct cmd_msg *cmd) {
	return send(socket, cmd, sizeof(*cmd), MSG_NOSIGNAL);
}

static inline ssize_t send_rsp(int socket, uint32_t val, const char *msg) {
	struct cmd_msg m = {
		.cmd = RSP,
		.rsp.val = val,
	};
	strncpy(m.rsp.msg, msg, sizeof(m.rsp.msg));
	return send_cmd(socket, &m);
}

static inline ssize_t send_hey(int socket, const char *name) {
	struct cmd_msg m = {
		.cmd = HEY,
	};
	strncpy(m.hey.name, name, sizeof(m.hey.name));
	return send_cmd(socket, &m);
}

static inline ssize_t process_hey(int socket, const char *username) {
	char msg[32];
	snprintf(msg, sizeof(msg), "Welcome %s", username);
	return send_rsp(socket, HEY, msg);
}

static inline ssize_t send_get(int socket, int x, int y) {
	struct cmd_msg m = {
		.cmd = GET,
		.get.x = x,
		.get.y = y,
	};
	return send_cmd(socket, &m);
}
static inline ssize_t process_get(int socket, int x, int y, int value) {

    struct cmd_msg m = {
		.cmd = GET,
		.get.x = x,
		.get.y = y,
		.get.val = value,
	};

	return send_cmd(socket, &m);
}
static inline ssize_t send_mov(int socket, float x, float y) {
	struct cmd_msg m = {
		.cmd = MOV,
		.mov.x = x,
		.mov.y = y,
	};
	return send_cmd(socket, &m);
}
static inline ssize_t send_pos(int socket, float x, float y) {
	struct cmd_msg m = {
		.cmd = POS,
		.pos.x = x,
		.pos.y = y,
	};
	return send_cmd(socket, &m);
}
