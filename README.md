![alt text](https://github.com/Bjond/pg_natural_sort_order/blob/master/images/bjondhealthlogo-whitegrey.png "Bjönd Inc.")

# PostgreSQL extension *pg_natural_sort_order*
=========================================

Provided by Bjönd Inc. free of charge.

A simple PostgreSQL extension to allow for natural sort order.
Basically a function is provided that allows a text string
to be mapped to another string that, when indexed, will allow
for proper natural sort order for large rows of data.

This extension was tested with PostgreSQL 9.5.4 on Linux.
It really should work on any version of 9.X PostgreSQL

Installation
============

There is a release for Linux provided in the github release area
that can be inserted into the extension shared library area of 
your postgreSQL installation. That is /usr/lib/postgresql/9.5/lib/
on my Ubuntu system. It varies per OS and Linux distribution.

Should you wish to build it yourself you will need the PostgreSQL development system installed.
That is beyond the scope of this document.

See the following to learn about PostgreSQL extensions:
http://linuxgazette.net/139/peterson.html
http://linuxgazette.net/142/peterson.html
https://www.postgresql.org/docs/9.5/static/xfunc-c.html

The key issue is that you need pg_config installed.


Build
============

Assuming the PostgreSQL pgxs system is installed:

```shell
    make; sudo make install
```

Then in a psql session issue:

```shell
    CREATE EXTENSION IF NOT EXISTS pg_natural_sort_order; SELECT natural_sort_order('a1111bz, 75);
```

Implementation
==============

natural_sort_order(string, integer)

Given an original string this method will normalize all numerics held within
that string (if any) to allow for Natural Sort Ordering.

https://en.wikipedia.org/wiki/Natural_sort_order
 
Algorithm:
For each numeric normalize it and construct a new string with the normalized
numeric in place of the original.

Normalization in this instance is that each numeric contain the same number of
places: 75 by default but you may choose a value between 1 and 150. 
Each numeric less than 75 (default) will be prepended with zeros. 

This algorithm can also be thought of as a mapping in which a numeric is
mapped to another numeric that always contains 75 places.

Any numeric greater than 75 results in a bad order and the numeric is split
which will probably result in a bad sort. Nothing can be done beyond increasing 
the normalization places. Highly unlikely for large values of N. 
The larger the N the more space is required for your index.

Sample Usage
============

Usually used in a trigger. Imagine a table that contains a *name* field
and you wish to construct another field with a normalized version based
upon *name* for every INSERT and UPDATE. This other field will be called
*nso_name* for _natural sort order name_.

```sql
CREATE FUNCTION natural_sort_order_function() RETURNS trigger AS $$
begin
new.nso_name :=  natural_sort_order(new.name, 75);
return new;
end $$ LANGUAGE plpgsql;

CREATE TRIGGER natrual_sort_order_trigger BEFORE INSERT OR UPDATE
ON our_table FOR EACH ROW EXECUTE PROCEDURE natural_sort_order_function();


CREATE INDEX natural_sort_order_index ON our_table (nso_name ASC);

```

Then all you need to do to order by natural sort order is:

```sql
SELECT * FROM our_table ORDER BY nso_name;
```




