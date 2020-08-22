#include "methods.h"
#include <string.h>
#include <stdio.h>
#include <json-c/json.h>
#include "sysinfo.h"
#include "hardwareinfo.h"

void systemInfo(char* result)
{
    struct json_object* jobj = json_object_new_object();
    json_object_object_add(jobj, "id", json_object_new_int(42 /* some id */));
    json_object_object_add(jobj, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(jobj, "method", json_object_new_string("systemInfo"));
    struct json_object* params = json_object_new_object();
    json_object_object_add(params, "ramTotal", json_object_new_int64(ramTotal()));
    json_object_object_add(params, "ramFree", json_object_new_int64(ramFree()));
    json_object_object_add(params, "procsNumber", json_object_new_int(procsNumber()));
    /* etc... */
    json_object_object_add(jobj, "params", params);
    const char* jsonstr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY);
    strcpy(result, jsonstr);
    json_object_put(jobj);
}

/* For example */
void hardwareInfo(char* result)
{
    struct json_object* jobj = json_object_new_object();
    json_object_object_add(jobj, "id", json_object_new_int(42 /* some id */));
    json_object_object_add(jobj, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(jobj, "method", json_object_new_string("hardwareInfo"));
    struct json_object* params = json_object_new_object();
    char m[10];
    machine(m);
    json_object_object_add(params, "Machine", json_object_new_string(m));
    /* etc... */
    json_object_object_add(jobj, "params", params);
    const char* jsonstr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY);
    strcpy(result, jsonstr);
    json_object_put(jobj);
}

typedef struct {
    const char* name;
    void (*call)(char*);
} method_t;

static method_t methods[] =
{
    { "systemInfo", systemInfo },
    { "hardwareInfo",  hardwareInfo }
    /* other methods */
};

static size_t methods_size = sizeof(methods) / sizeof(method_t);

void doMethodByName(const char* const name, char* result)
{
    if (result == NULL) {
        return;
    }

    for (size_t i = 0; i < methods_size; ++i) {
        if (strcmp(name, methods[i].name) == 0) {
            methods[i].call(result);
            break;
        }
    }
}

void printMethodsList()
{
    for (size_t i = 0; i < methods_size; ++i) {
        printf("* %s\n", methods[i].name);
    }
}
