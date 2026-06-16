CREATE OR REPLACE FUNCTION pgl_ddl_deploy.set_tag_defaults()
 RETURNS trigger
 LANGUAGE plpgsql
AS $function$
BEGIN
IF NEW.create_tags IS NULL THEN
    NEW.create_tags = CASE WHEN NEW.include_only_repset_tables THEN pgl_ddl_deploy.standard_repset_only_tags() ELSE pgl_ddl_deploy.standard_create_tags() END;
    IF NEW.include_indexes AND NOT NEW.include_only_repset_tables THEN
        NEW.create_tags = NEW.create_tags || '{"CREATE INDEX","ALTER INDEX"}'::TEXT[];
    END IF;
END IF;
IF NEW.drop_tags IS NULL THEN
    NEW.drop_tags = CASE WHEN NEW.include_only_repset_tables THEN NULL ELSE pgl_ddl_deploy.standard_drop_tags() END;
    IF NEW.include_indexes AND NOT NEW.include_only_repset_tables THEN
        NEW.drop_tags = NEW.drop_tags || '{"DROP INDEX"}'::TEXT[];
    END IF;
END IF;
RETURN NEW;
END;
$function$
;