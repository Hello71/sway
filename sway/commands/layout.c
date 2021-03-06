#include <string.h>
#include <strings.h>
#include "sway/commands.h"
#include "sway/tree/container.h"
#include "sway/tree/layout.h"
#include "log.h"

struct cmd_results *cmd_layout(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "layout", EXPECTED_MORE_THAN, 0))) {
		return error;
	}
	struct sway_container *parent = config->handler_context.current_container;

	// TODO: floating
	/*
	if (parent->is_floating) {
		return cmd_results_new(CMD_FAILURE, "layout", "Unable to change layout of floating windows");
	}
	*/

	while (parent->type == C_VIEW) {
		parent = parent->parent;
	}

	// TODO: stacks and tabs

	if (strcasecmp(argv[0], "default") == 0) {
		container_set_layout(parent, parent->prev_layout);
		if (parent->layout == L_NONE) {
			container_set_layout(parent, container_get_default_layout(parent));
		}
	} else {
		if (parent->layout != L_TABBED && parent->layout != L_STACKED) {
			parent->prev_layout = parent->layout;
		}

		if (strcasecmp(argv[0], "splith") == 0) {
			container_set_layout(parent, L_HORIZ);
		} else if (strcasecmp(argv[0], "splitv") == 0) {
			container_set_layout(parent, L_VERT);
		} else if (strcasecmp(argv[0], "toggle") == 0 && argc == 2 && strcasecmp(argv[1], "split") == 0) {
			if (parent->layout == L_HORIZ && (parent->workspace_layout == L_NONE
					|| parent->workspace_layout == L_HORIZ)) {
				container_set_layout(parent, L_VERT);
			} else {
				container_set_layout(parent, L_HORIZ);
			}
		}
	}

	arrange_windows(parent, parent->width, parent->height);

	return cmd_results_new(CMD_SUCCESS, NULL, NULL);
}
