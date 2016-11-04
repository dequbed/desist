#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "libvici.h"

int main(int argc, char *argv[])
{
    vici_init();

    vici_conn_t *conn = vici_connect(NULL);

    if (conn == NULL)
    {
        printf("%d\n", errno);
        exit(-1);
    }

    vici_req_t *req = vici_begin("load-conn");

    char* name = malloc(22);
    snprintf(name, 22, "spoke-%s", argv[1]);

    vici_begin_section(req, name);

        vici_begin_list(req, "remote_addrs");
        vici_add_list_itemf(req, "%s", argv[1]);
        vici_end_list(req);

        vici_begin_section(req, "local");
            vici_add_key_value(req, "auth", "pubkey", 6);
            vici_begin_list(req, "certs");

            FILE* f = fopen(argv[2], "r");
            fseek(f, 0, SEEK_END);
            uint64_t len = ftell(f);
            rewind(f);
            char* cert = malloc(len+1);
            fread(cert, len, 1, f);
            fclose(f);
            cert[len] = 0;

            vici_add_list_item(req, cert, len);

            vici_end_list(req);
        vici_end_section(req);

        vici_begin_section(req, "remote");
            vici_add_key_value(req, "auth", "pubkey", 6);
        vici_end_section(req);

        vici_begin_section(req, "children");
            vici_begin_section(req, "test2");
            vici_begin_list(req, "remote_ts");
            vici_add_list_itemf(req, "%s", "10.84.0.0/16");
            vici_end_list(req);
            vici_end_section(req); //test2
        vici_end_section(req); //children

    vici_end_section(req); //test

    vici_res_t *res = vici_submit(req, conn);

    if (res == NULL)
    {
        printf("%d\n", errno);
        exit(1);
    }

    int resi = vici_dump(res, "test", 0, stdout);

    vici_disconnect(conn);

    vici_deinit();
    return resi;
}
