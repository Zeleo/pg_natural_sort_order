#!/bin/sh
/bin/mkdir -p '/usr/lib/postgresql/9.4/lib'
/bin/mkdir -p '/usr/share/postgresql/9.4/extension'
/bin/mkdir -p '/usr/share/postgresql/9.4/extension'
/usr/bin/install -c -m 755  pg_natural_sort_order.so '/usr/lib/postgresql/9.4/lib/pg_natural_sort_order.so'
/usr/bin/install -c -m 644 .//pg_natural_sort_order.control '/usr/share/postgresql/9.4/extension/'
/usr/bin/install -c -m 644 .//pg_natural_sort_order--1.0.sql  '/usr/share/postgresql/9.4/extension/'
