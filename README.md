WIP readme

Install dcmtk for Linux:
```sudo apt install libdcmtk-dev```

Configure directories with cmake

```cmake -B/DICOM-build/ -S .```

Build with

```cmake --build ../DICOM-build```

Run with

```./../DICOM-build/dicompractice```