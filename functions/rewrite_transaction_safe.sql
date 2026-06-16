CREATE OR REPLACE FUNCTION pgl_ddl_deploy.rewrite_transaction_safe(p_sql text)
 RETURNS text
 LANGUAGE c
 STRICT
AS '$libdir/pgl_ddl_deploy', $function$rewrite_transaction_safe$function$
;