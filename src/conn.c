#include "conn.h"

void add_list(vici_req_t* req, char* type, char* elements[], uint32_t len)
{
    vici_begin_list(req, type);

    for (uint32_t i = 0; i < len; i++)
        vici_add_list_itemf(req, elements[i]);

    vici_end_list(req);
}

int setup(char* remote)
{
    char* proposals[] = {"chacha20poly1305-aes256gcm16-prfsha384-ecp384bp-modp2048s256"};
    char* remote_addrs[] = {remote};

    FILE* f = fopen("/etc/swanctl/x509/mesh.der", "r");
    fseek(f, 0, SEEK_END);
    uint32_t len = ftell(f);
    rewind(f);
    char* cert = calloc(1, len);
    fread(cert, len, 1, f);
    fclose(f);
    //cert[len] = 0;

    char* esp_proposals[] = {"chacha20poly1305", "aes256gcm16"};
    char* local_ts[] = {"dynamic[gre]"};

    vici_init();

    vici_conn_t* conn = vici_connect(NULL);

    if (conn == NULL)
    {
        printf("%d\n", errno);
        exit(-1);
    }

    vici_req_t* req = vici_begin("load-conn");

    char* name = calloc(1, 15);
    snprintf(name, 14, "spoke-%X", hash((uint8_t*)remote, strlen(remote)));

    vici_begin_section(req, name);

    vici_add_key_valuef(req, "version", "2");
    vici_add_key_valuef(req, "mobike", "no");

    add_list(req, "proposals", proposals, 1);
    add_list(req, "remote_addrs", remote_addrs, 1);

    vici_begin_section(req, "local");
    vici_add_key_valuef(req, "auth", "pubkey");

    vici_begin_list(req, "certs");
    vici_add_list_item(req, cert, len);
    vici_end_list(req);

    vici_end_section(req);

    vici_begin_section(req, "remote");
    vici_add_key_valuef(req, "auth", "pubkey");
    vici_end_section(req);

    vici_begin_section(req, "children");
    vici_begin_section(req, "transport");
    add_list(req, "local_ts", local_ts, 1);
    vici_add_key_valuef(req, "updown", "/usr/lib/strongswan/_updown");
    vici_add_key_valuef(req, "mode", "transport");
    add_list(req, "esp_proposals", esp_proposals, 2);
    vici_add_key_valuef(req, "priority", "2");
    vici_end_section(req);
    vici_end_section(req);

    vici_end_section(req);

    vici_res_t* res = vici_submit(req, conn);

    if (res == NULL)
    {
        printf("%d\n", errno);
        exit(1);
    }

    uint32_t resi = vici_dump(res, "test", 0, stdout);

    vici_disconnect(conn);

    vici_deinit();
    return resi;
}
