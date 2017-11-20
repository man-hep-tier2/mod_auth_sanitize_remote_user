/*
Copyright 2016 Robert W Frank

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "httpd.h"
#include "http_config.h"
#include "http_request.h"
#include "ap_config.h"
#include "http_log.h"
#include "apr_strings.h"

#define ASRU_DEFAULT_ACCEPT " "
#define ASRU_ACCEPT_LENGTH 32

typedef struct {
	char accept[ASRU_ACCEPT_LENGTH];
	char replacement;
} asru_cfg;

static void register_hooks(apr_pool_t*);
static int sanitize_handler(request_rec*);
const char* asru_set_replacechars(cmd_parms*, void*, const char*, const char*);
void* asru_create_dir_config(apr_pool_t*, char*);
void* asru_merge_dir_config(apr_pool_t*, void*, void*);

static const command_rec asru_cmds[] =
{
	AP_INIT_TAKE2("RemoteUserReplaceChars", asru_set_replacechars, NULL, RSRC_CONF, "A set of characters to replace, and a character to replace them with"),
	{NULL}
};

module AP_MODULE_DECLARE_DATA auth_sanitize_remote_user_module =
{
    STANDARD20_MODULE_STUFF,
    asru_create_dir_config, /* dir config creater */
    asru_merge_dir_config,  /* dir merger --- default is to override */
    NULL,                   /* server config */
    NULL,                   /* merge server configs */
    asru_cmds,              /* command apr_table_t */
    register_hooks          /* register hooks */
};

static void register_hooks(apr_pool_t *p)
{
    ap_hook_handler(sanitize_handler, NULL, NULL, APR_HOOK_LAST);
}

static int sanitize_handler(request_rec *r)
{
	asru_cfg *cfg = (asru_cfg*)ap_get_module_config(r->per_dir_config, &auth_sanitize_remote_user_module);
	char *p = r->user;
	if(p != NULL)
		while((p = strpbrk(p, cfg->accept)) != NULL)
			*p = cfg->replacement;
	return DECLINED;
}

const char* asru_set_replacechars(cmd_parms *cmd, void *config, const char *accept, const char *replace)
{
	asru_cfg *cfg = (asru_cfg*)config;
	if(cfg)
	{
		apr_cpystrn(cfg->accept, accept, sizeof(cfg->accept));
		cfg->replacement = replace[0];
	}
	return NULL;
}

void* asru_create_dir_config(apr_pool_t *pool, char *context)
{
	asru_cfg *cfg = apr_pcalloc(pool, sizeof(asru_cfg));
	if(cfg)
	{
		apr_cpystrn(cfg->accept, ASRU_DEFAULT_ACCEPT, sizeof(cfg->accept));
		cfg->replacement = '_';
	}
	return cfg;
}

void* asru_merge_dir_config(apr_pool_t *pool, void *base, void *add)
{
	asru_cfg *cfg = (asru_cfg*)asru_create_dir_config(pool, NULL);
	if(cfg)
	{
		asru_cfg *basecfg = (asru_cfg*)base, *addcfg = (asru_cfg*)add;
		char *acceptsrc = (apr_strnatcmp(basecfg->accept, addcfg->accept) ? addcfg->accept : basecfg->accept);
		apr_cpystrn(cfg->accept, acceptsrc, sizeof(cfg->accept));
		cfg->replacement = (basecfg->replacement != addcfg->replacement ? addcfg->replacement : basecfg->replacement);
	}
	return cfg;
}
