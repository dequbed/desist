#include "libvici.h"
#include <stdlib.h>
#include <errno.h>

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

    vici_begin_section(req, "test");

        vici_begin_list(req, "remote_addrs");
        vici_add_list_itemf(req, "%s", "hub.paranoidlabs.org");
        vici_end_list(req);

        vici_begin_section(req, "local");
            vici_add_key_value(req, "auth", "pubkey", 6);
            vici_begin_list(req, "certs");
            vici_add_list_itemf(req, "%s", "aicube.crt");
            vici_end_list(req);
        vici_end_section(req);

        vici_begin_section(req, "remote");
            vici_add_key_value(req, "auth", "pubkey", 6);
            vici_add_key_valuef(req, "id", "%s", "hub.paranoidlabs.org");
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
