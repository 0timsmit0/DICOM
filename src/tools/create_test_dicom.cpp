#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string path = (argc >= 2) ? argv[1] : "test_green_square.dcm";

    const int width = 100;
    const int height = 100;
    const int numPixels = width * height;

    DcmFileFormat fileformat;
    DcmDataset* dataset = fileformat.getDataset();

    dataset->putAndInsertString(DCM_PatientName, "Test^Patient");
    dataset->putAndInsertString(DCM_PatientID, "123456");
    dataset->putAndInsertUint16(DCM_Rows, height);
    dataset->putAndInsertUint16(DCM_Columns, width);
    dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
    dataset->putAndInsertUint16(DCM_BitsStored, 8);
    dataset->putAndInsertUint16(DCM_HighBit, 7);
    dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
    dataset->putAndInsertString(DCM_PhotometricInterpretation, "RGB");
    dataset->putAndInsertUint16(DCM_SamplesPerPixel, 3);

    std::vector<Uint8> pixelData(numPixels * 3);
    for (int i = 0; i < numPixels; ++i) {
        pixelData[i * 3 + 0] = 0;   // Red
        pixelData[i * 3 + 1] = 255; // Green
        pixelData[i * 3 + 2] = 0;   // Blue
    }

    dataset->putAndInsertUint8Array(DCM_PixelData, pixelData.data(), pixelData.size());

    OFCondition status = fileformat.saveFile(path.c_str(), EXS_LittleEndianExplicit);
    if (status.good()) {
        std::cout << "Created: " << path << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to save: " << status.text() << std::endl;
        return 1;
    }
}
