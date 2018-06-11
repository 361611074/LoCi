#include "ipdb.h"
#include "util.h"

ipdb* ipdb_create(const ipdb_handle *handle, const uint8_t *buffer, uint32_t length, void *extend)
{
    ipdb *db = (ipdb *)calloc(1, sizeof(ipdb));
    db->handle = handle;
    db->buffer = buffer;
    db->length = length;
    db->extend = extend;
    if(db->handle->init)
    {
        if(db->handle->init(db))
        {
            return db;
        }
    }
    ipdb_release(db);
    return NULL;
}

void ipdb_release(ipdb *db)
{
    if(db->handle->quit)
    {
        db->handle->quit(db);
    }
    free(db);
}

bool ipdb_find(const ipdb *db, ipdb_item *item, const char *ip)
{
    if(db->handle->find)
    {
        return db->handle->find(db, item, str2ip(ip));
    }
    return false;
}

bool ipdb_next(ipdb_iter *iter, ipdb_item *item)
{
    if(iter->db->handle->iter)
    {
        return iter->db->handle->iter(iter->db, item, iter->index++);
    }
    return false;
}

bool ipdb_dump(const ipdb *db, const char *file)
{
    FILE *fp = fopen(file, "wb");
    if(fp)
    {
        ipdb_iter iter = {db, 0};
        ipdb_item item;
        while(ipdb_next(&iter, &item))
        {
            char ip1[16];
            char ip2[16];

            char *ip1_t = ip2str(ip1, sizeof(ip1), item.lower);
            char *ip2_t = ip2str(ip2, sizeof(ip2), item.upper);
            fprintf(fp, "%-16s%-16s%s%s%s\r\n", ip1_t, ip2_t, item.zone, item.area[0]?" ":"", item.area);
        }

        //fprintf(fp, "\r\n\r\nIP数据库共有数据 ： %d 条\r\n", db->count); /* 为了和纯真解压一致，需要本文件编码为GB2312 */
        fclose(fp);
        return true;
    }
    return false;
}


static uint8_t* readfile(const char *path, uint32_t *length)
{
	uint8_t* buffer = 0;
	FILE *fp = nullptr;
	fopen_s(&fp, path, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		*length = ftell(fp);
		buffer = (uint8_t*)malloc(*length);

		fseek(fp, 0, SEEK_SET);
		fread(buffer, *length, 1, fp);
		fclose(fp);
	}
	return buffer;
}

char* ipdb_read(int db_type, const char *db_path, const char *ip_addr)
{
	ipdb_handle *handle = 0;
	switch (db_type)
	{
	case 0:
		handle = (ipdb_handle*)&qqwry_handle;
		break;
	case 1:
		handle = (ipdb_handle*)&mon17_handle;
		break;
	case 2:
		handle = (ipdb_handle*)&txtdb_handle;
		break;
	default:
		handle = (ipdb_handle*)&qqwry_handle;
		break;
	}
	uint32_t length = 0;
	uint8_t *buffer = readfile(db_path, &length);
	ipdb *db = ipdb_create(handle, buffer, length, NULL);
	if (db == NULL) return _strdup("ipdb_create_error");
	ipdb_item item;
	if (ipdb_find(db, &item, ip_addr) == false) return _strdup("ipdb_find_error");
	char loca[256];
	sprintf(loca, "%s %s", item.zone, item.area);
	ipdb_release(db);
	if (buffer) free(buffer);
	return _strdup(loca);
}
