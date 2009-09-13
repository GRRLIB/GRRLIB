*** Temporary fix ***
After the release of libogc v1.7.1a, Vector was renamed to guVector
"to avoid collisions" [svn 3650]
...also Quaternion was renamed to guQuaternion - but GRRLIB does not use these!

The main codebase of GRRLIB has been updated to reflect this change.
But until the new libogc is officially released,
if you are using a version of libogc later than v1.7.1a/svn3649,
you will need to add:
  -DNOGUFIX
to the compiler flags in your makefile

When the libogc changes are officially released,
this *temporary fix* should be removed [see grrlib.h, line 50ish]
The requirement for -DNOGUFIX will be deprecated,
but its lingering presence will not be a hinderance

Thanks to Nicksasa for reporting this problem :)
