/* flux-lsmod.c - flux lsmod subcommand */

#define _GNU_SOURCE
#include <getopt.h>
#include <json/json.h>
#include <assert.h>
#include <libgen.h>

#include "cmb.h"
#include "util.h"
#include "log.h"

#define OPTIONS "hl"
static const struct option longopts[] = {
    {"help",       no_argument,        0, 'h'},
    {"long",       no_argument,        0, 'l'},
    { 0, 0, 0, 0 },
};

static void list_module (bool lopt, const char *key, json_object *mo);

void usage (void)
{
    fprintf (stderr, 
"Usage: flux-lsmod [--long]\n"
);
    exit (1);
}

int main (int argc, char *argv[])
{
    flux_t h;
    int ch;
    json_object *mods;
    json_object_iter iter;
    bool lopt = false;

    log_init ("flux-rmmod");

    while ((ch = getopt_long (argc, argv, OPTIONS, longopts, NULL)) != -1) {
        switch (ch) {
            case 'h': /* --help */
                usage ();
                break;
            case 'l': /* --long */
                lopt = true;
                break;
            default:
                usage ();
                break;
        }
    }
    if (optind != argc)
        usage ();

    if (!(h = cmb_init ()))
        err_exit ("cmb_init");

    if (!(mods = flux_lsmod (h)))
        err_exit ("flux_lsmod");

    json_object_object_foreachC (mods, iter) {
        list_module (lopt, iter.key, iter.val);
    }
    json_object_put (mods);

    flux_handle_destroy (&h);
    log_fini ();
    return 0;
}

static void list_module (bool lopt, const char *key, json_object *mo)
{
    const char *name;
    json_object *args;
    json_object_iter iter;

    if (util_json_object_get_string (mo, "name", &name) < 0
            || !(args = json_object_object_get (mo, "args")))
        msg_exit ("error parsing lsmod response");

    printf ("%-10.10s ", key);
    if (lopt) {
        printf ("%-10.10s ", name);
        json_object_object_foreachC (args, iter) {
            const char *val = json_object_get_string (iter.val);
            if (val)
                printf ("%s=%s ", iter.key, val);
            else
                printf ("%s ", iter.key);
        }
    }
    printf ("\n");
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */