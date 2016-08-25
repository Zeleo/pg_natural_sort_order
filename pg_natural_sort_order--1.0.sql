-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_natural_sort_order" to load this file. \quit

CREATE OR REPLACE FUNCTION natural_sort_order( TEXT, INTEGER ) RETURNS TEXT
  AS 'pg_natural_sort_order' LANGUAGE C;
