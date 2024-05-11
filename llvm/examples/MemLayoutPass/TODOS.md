
# TODOS

- [x] Generate executable from memlayout_tests.cpp
- [ ] Experiment with running the memlayout pass before vs after -O3, observe the results
- [ ] Make memlayout plugin into a dynamically loaded module in order to (massively) save on compilation time
- [ ] Use clang to build itself, to avoid the comically slow linker times
- [ ] Find all upwards-exposed memory accesses
- [ ] Target this pass at a custom pragma


