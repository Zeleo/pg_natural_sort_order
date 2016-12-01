![alt text](https://github.com/Bjond/pg_natural_sort_order/blob/master/images/bjondhealthlogo-whitegrey.png "Bjönd Inc.")

# PostgreSQL extension *pg_natural_sort_order*
=============================================

Provided by Bjönd Inc. free of charge. MIT License

A simple PostgreSQL extension to allow for natural sort order.
Basically a function is provided that allows a text string
to be mapped to another string that, when indexed, will allow
for proper natural sort order for large rows of data.

This extension was tested with PostgreSQL 9.5.4 on Linux.
It really should work on any version of 9.X PostgreSQL

NOTE: For Window's binaries see the chapter *Window's Binaries*
towards the end of this document.

Installation
============

There is a release for Linux provided in the github release area
that can be inserted into the extension shared library area of 
your postgreSQL installation. That is /usr/lib/postgresql/9.5/lib/
on my Ubuntu system. It varies per OS and Linux distribution.

Should you wish to build it yourself you will need the PostgreSQL development system installed.
That is beyond the scope of this document.

See the following to learn about PostgreSQL extensions:

[Peterson, Article 1, Linux Gazette: June 2007 ](http://linuxgazette.net/139/peterson.html)

[Peterson, Article 2, Linux Gazette: June 2007](http://linuxgazette.net/142/peterson.html)

[PostgreSQL Documentation Version 9.5](https://www.postgresql.org/docs/9.5/static/xfunc-c.html)


The key issue is that you need pg_config installed.


Build
=====

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

Normalization in this instance is that each numeric contains the same number of
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


Binaries 
============

Within the _binaries_ directory are contained the various shared libraries
for Windows, MacOS and Linux. In addition a Windows 2015 Express project
has been included for the production of the Windows DLL from scratch 
should you wish to do so. 

Window's Binaries
================

Windows, as per always, is a chore. Not only is the creation of the shared libraries
different but you also need to ensure you install additonal Visual Studio dll's.
Those additional libraries can be installed via _Visual C++ Redistributable for Visual Studio 2015_.

I'm a Linux dev and not a Windows developer (thank the Gods) so this work was done via the good
people of the Republic of Germany. There is a subdirectory called _windows_ off of root that contains
a zip file of several versions of the DLL suitable for use as long as you install the Visual C++
distributable libraries.

Here is the snippet of the email describing the particulars:


```
Hi Stephen,
please find attached the ddl's for Postgresql 9.2, 9.3, 9.4, 95. and 9.6.

Here is how I generated them:
I compiled them using the latest installations available for the releases (9.2.19, 9.3.15, 9.4.10, 9.5.5 and 9.6.1) at https://www.postgresql.org/download/windows/ .
For the compilation I used Visual Studio Community 2015, Version 14..0.25431.01 Update 3 and the project you provide in your download. The dll's are generated for the configuration release and the x64 platform.
My platform for this task is Windows 10 64 bit (Version 1607).
For the postgresql releases I changed the settings for additional include directories (C/C++ in the Configuration Parameters) and the additional library directories (Linker in the Configuration Parameters) to the paths in the particular installation folder for the postgresql-release.
On fresh systems the necessary "Visual C++ Redistributable für Visual Studio 2015" have to be installed, otherwise you will get an error message when creating the extension for a database. The installation file can be downloaded here:
https://www.microsoft.com/de-de/download/details.aspx?id=48145

best regards,
Guido
-- 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Guido Heinz M.Eng.
* Römisch-Germanisches Zentralmuseum
* Leibniz-Forschungsinstitut für Archäologie
* Ernst-Ludwig-Platz 2, 55116 Mainz
* Tel: +49-6131-628-1486
* mailto:heinz@rgzm.de

Mitglied der Leibniz-Gemeinschaft 
```




---
[MIT License](https://en.wikipedia.org/wiki/MIT_License) &copy;

[Bjönd Inc.](http://www.bjondinc.com/)

![alt text](https://github.com/Bjond/pg_natural_sort_order/blob/master/images/postgres.png "PostgreSQL.")
