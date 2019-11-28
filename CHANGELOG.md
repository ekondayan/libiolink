# <u>0.1.0</u>

Initial release. Almost all parts of the library are usable, but is not recommended for production at this stage. The following check list must be completed before the library gets ready for production.

- [ ] Every file must include all the headers that it uses. It means that it must be able to be compiled as stand alone unit.

- [ ] Reorganize the filesystem structure

- [ ] Reorganize the namespaces

- [ ] TimeSpanT is broken. Must be implemented correctly

- [ ] OctetStringT is broken. Must be implemented correctly

- [ ] Implement unpackFromVector() for TimeSpanT, TimeT, StringT, OctetStringT

- [ ] Implement packToVector() for all IODD types

- [ ] Implement RecordT

- [ ] Implement subindex_access for ArrayT and RecordT

- [ ] Stabilize the API


