# ADBMS Semester Project - Phase 2
## PostgreSQL semver Extension

### Team Members & Contributions
- **Student A (Ezzah)**: Base Type + Input/Output
- **Student B(Eiman)**: Comparison Operators + Aggregates
- **Student C (Hamza)**: **Utility Functions + Regression Tests + README + Demo**

### Features Added by Student C
- `major(semver)` 
- `minor(semver)`
- `patch(semver)`
- `bump_minor(semver)`
- `is_compatible(semver, semver)`

### Build & Install
```bash
cd semver_ext
make clean
make
sudo make install
