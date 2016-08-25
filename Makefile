MODULE_big = pg_natural_sort_order
OBJS = pg_natural_sort_order.o

EXTENSION = pg_natural_sort_order
DATA = pg_natural_sort_order--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
