#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "libvici.h"

void add_list(vici_req_t *req, char* type, char* proposals[]) {
    vici_begin_list(req, type);
    int i;
    for(i=0; i < sizeof(proposals)/sizeof(proposals[0]); i++)
        vici_add_list_itemf(req, proposals[0]);
    vici_end_list(req);
}

int main(int argc, char *argv[])
{
    char* proposals[] = {"chacha20poly1305-aes256gcm16-prfsha384-ecp384bp-modp2048s256"};
    char* remote_addrs[] = {argv[1]};

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

        vici_add_key_valuef(req, "version", "2");
        vici_add_key_valuef(req, "mobike", "no");

        add_list(req, "proposals", proposals);
        /*vici_begin_list(req, "proposals");
        vici_add_list_itemf(req, "chacha20poly1305-aes256gcm16-prfsha384-ecp384bp-modp2048s256");
        vici_end_list(req);*/

	add_list(req, "remote_addrs", remote_addrs);
        /*vici_begin_list(req, "remote_addrs");
        vici_add_list_itemf(req, "%s", argv[1]);
        vici_end_list(req);*/

        vici_begin_section(req, "local");
            vici_add_key_valuef(req, "auth", "pubkey");
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
            vici_add_key_valuef(req, "auth", "pubkey");
        vici_end_section(req);

        vici_begin_section(req, "children");
            vici_begin_section(req, "transport");
                vici_begin_list(req, "local_ts");
                    vici_add_list_itemf(req, "dynamic[gre]");
                vici_end_list(req);
                vici_add_key_valuef(req, "updown", "/usr/lib/strongswan/_updown");
                vici_add_key_valuef(req, "mode", "transport");

            vici_begin_list(req, "esp_proposals");
                vici_add_list_itemf(req, "chacha20poly1305");
                vici_add_list_itemf(req, "aes256gcm16");
            vici_end_list(req);

                vici_add_key_valuef(req, "priority", "2");
            vici_end_section(req);
        vici_end_section(req);

    vici_end_section(req);

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
