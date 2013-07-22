/* velox: modules/desktop.c
 *
 * Copyright (c) 2013 Michael Forney <mforney@mforney.org>
 *
 * This file is a part of velox.
 *
 * velox is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2, as published by the Free
 * Software Foundation.
 *
 * velox is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with velox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <yaml.h>
#include <velox/velox.h>
#include <velox/workspace.h>
#include <velox/hook.h>

static void handle_floating(union velox_argument argument);
static void handle_fullscreen(union velox_argument argument);

const char name[] = "desktop";

void configure(yaml_document_t * document)
{
    yaml_node_t * map, * key, * value;
    yaml_node_pair_t * pair;

    printf("Desktop: Loading configuration...");

    map = yaml_document_get_root_node(document);
    assert(map->type == YAML_MAPPING_NODE);

    for (pair = map->data.mapping.pairs.start;
        pair < map->data.mapping.pairs.top;
        ++pair)
    {
        key = yaml_document_get_node(document, pair->key);
        value = yaml_document_get_node(document, pair->value);

        assert(key->type == YAML_SCALAR_NODE);
    }

    printf("done\n");
}

bool setup()
{
    uint32_t index;
    const char * workspace_names[] =
        { "term", "www", "irc", "im", "code", "mail", "gfx", "music", "misc" };
    const char * default_layouts[] = { "tile", "grid", NULL };

    printf("Desktop: Initializing...");

    /* Workspaces */
    for (index = 0; index < ARRAY_LENGTH(workspace_names); ++index)
    {
        add_workspace(workspace_names[index], default_layouts);
    }

    /* Hooks */
    add_hook(&handle_floating, VELOX_HOOK_MANAGE_PRE);
    add_hook(&handle_fullscreen, VELOX_HOOK_MANAGE_PRE);

    printf("done\n");

    return true;
}

void cleanup()
{
    printf("Desktop: Cleaning up...");
    printf("done\n");
}

/* Manage hooks */
static void handle_floating(union velox_argument argument)
{
    struct velox_window * window = (struct velox_window *) argument.pointer;

    /* TODO: Make download konqueror windows floating */
    if (strcmp(window->name, "MPlayer") == 0)
    {
        window->floating = true;
    }
    else if (strcmp(window->name, "xclock") == 0)
    {
        window->floating = true;
    }
}

static void handle_fullscreen(union velox_argument argument)
{
    struct velox_window * window = (struct velox_window *) argument.pointer;

    /*
    if (window->width == screen_area.width && window->height == screen_area.height)
    {
        window->x = 0;
        window->y = 0;
        window->border_width = 0;
        window->floating = true;
    }
    */
}

// vim: fdm=syntax fo=croql et sw=4 sts=4 ts=8

