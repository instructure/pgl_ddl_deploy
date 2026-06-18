SET client_min_messages = warning;

CREATE PUBLICATION test_indexes_yes;
CREATE PUBLICATION test_indexes_no;

INSERT INTO pgl_ddl_deploy.set_configs (set_name, include_schema_regex, lock_safe_deployment, allow_multi_statements, include_indexes)
VALUES ('test_indexes_yes', '^yindex.*', true, true, true);

INSERT INTO pgl_ddl_deploy.set_configs (set_name, include_schema_regex, lock_safe_deployment, allow_multi_statements, include_indexes)
VALUES ('test_indexes_no', '^nindex.*', true, true, false);

SELECT pgl_ddl_deploy.deploy('test_indexes_yes');
SELECT pgl_ddl_deploy.deploy('test_indexes_no');

CREATE SCHEMA yindex;
CREATE TABLE yindex.widgets(id serial primary key, type varchar);
CREATE SCHEMA nindex;
CREATE TABLE nindex.widgets(id serial primary key, type varchar);

CREATE INDEX yindex_widgets_type_idx ON yindex.widgets(type);
CREATE INDEX CONCURRENTLY nindex_widgets_type_id_idx ON yindex.widgets(type, id);
CREATE INDEX nindex_widgets_type_idx ON nindex.widgets(type);

DROP TABLE yindex.widgets CASCADE;
DROP TABLE nindex.widgets CASCADE;

-- We expect to see index events for yindex.widgets but not for nindex.widgets
SELECT c.set_name, ddl_sql_raw, ddl_sql_sent, c.ddl_only_replication
FROM pgl_ddl_deploy.events e
INNER JOIN pgl_ddl_deploy.set_configs c ON c.id = e.set_config_id
ORDER BY e.id DESC LIMIT 10;

SET client_min_messages TO warning;
DROP OWNED BY test_pgl_ddl_deploy;
DROP ROLE test_pgl_ddl_deploy;
DROP ROLE unpriv;
DROP EXTENSION pgl_ddl_deploy CASCADE;
DROP EXTENSION IF EXISTS pglogical CASCADE;
DROP SCHEMA IF EXISTS pglogical CASCADE;
DROP TABLE IF EXISTS tmp_objs;
DROP SCHEMA IF EXISTS special CASCADE;
DROP SCHEMA IF EXISTS bla CASCADE;
DROP SCHEMA IF EXISTS pgl_ddl_deploy CASCADE;
