# Versioning

## 2.A.B.C
- A = MAJOR version, when rAthena src patch is required
- B = MINOR version, when functionality is added.
- C = PATCH version, when a bug is fixed.

### Examples:
- 2.0.0.0 is the release version.
- 2.0.0.0 -> 2.1.0.0: something changed and it requires to apply a patch to rAthena sources to make it work.
- 2.1.0.0 -> 2.1.1.0: some functionality is added but it can be updated without changing rAthena source.
- 2.1.0.0 -> 2.1.0.1: a bug was fixed (backwards compatible).

### Updating roCORD
If you want to update roCORD and the major version (A) did not change than you can just copy paste the new roCORD src files into <rA-folder>/src/map/discord.
If the major version (A) changed you have to apply a patch to make the bot work.
----
#### Reference
https://semver.org
