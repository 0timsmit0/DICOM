#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>

#include <gtest/gtest.h>

#include <cstdio>
#include <stdexcept>
#include <vector>

class DICOMImageTest : public ::testing::Test {
    protected:
        const std::string testImagePath = "test_green_square.dcm";

        void SetUp() override {
            createGreenDICOMImage(testImagePath);
        }

        void TearDown() override {
            std::remove(testImagePath.c_str());
        }

        void createGreenDICOMImage(const std::string& path) {
            const int width = 100;
            const int height = 100;
            const int numPixels = width * height;

            DcmFileFormat fileformat;
            DcmDataset *dataset = fileformat.getDataset();

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

            dataset->putAndInsertUint8Array(DCM_PixelData, &pixelData[0], pixelData.size());

            fileformat.saveFile(path.c_str(), EXS_LittleEndianExplicit);
        }

        // Helper struct for RGB pixel values
        struct RGBPixel {
            uint8_t r, g, b;
        };

        // Helper function to read a pixel at a specific location
        RGBPixel getPixelAt(DcmDataset* dataset, int row, int col) {
            // TODO: Implement this function
            // 1. Get image dimensions (rows, cols)
            // 2. Get pixel data array using findAndGetUint8Array()
            // 3. Calculate pixel index: (row * cols + col) * 3
            // 4. Return RGB values from the array

            RGBPixel pixel = {0, 0, 0};
            Uint16 cols = 0;
            unsigned long count = 0;
            
            dataset->findAndGetUint16(DCM_Columns, cols);
            
            const Uint8 *pixelData = nullptr;
            dataset->findAndGetUint8Array(DCM_PixelData, pixelData, &count);
            if (count < (row*cols+col)*3 + 2) {
                throw std::runtime_error("Pixel index out of bounds");
            }
            int index = (row * cols + col) * 3;
            pixel.r = pixelData[index];
            pixel.g = pixelData[index + 1];
            pixel.b = pixelData[index + 2];
            return pixel;
        }

        std::string getMetadataString(DcmDataset* dataset, const DcmTagKey& tag) {
            OFString value;
            if (dataset->findAndGetOFString(tag, value).good()) {
                return value.c_str();
            }
            return "";
        }
};

// Test 1: Verify that the DICOM file can be loaded without errors
TEST_F(DICOMImageTest, CanReadDicomFile) {
    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(testImagePath.c_str());
    ASSERT_TRUE(status.good()) << "Failed to load DICOM file: " << status.text();
}

// Test 2: Verify that metadata is correctly stored and retrieved
TEST_F(DICOMImageTest, MetadataIsCorrect) {
    
    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(testImagePath.c_str());
    ASSERT_TRUE(status.good()) << "Failed to load DICOM file: " << status.text();
    DcmDataset* dataset = fileformat.getDataset();
    ASSERT_EQ(getMetadataString(dataset,DCM_PatientName), "Test^Patient");
    ASSERT_EQ(getMetadataString(dataset,DCM_PatientID), "123456");
    ASSERT_EQ(getMetadataString(dataset,DCM_Rows), "100");
    ASSERT_EQ(getMetadataString(dataset,DCM_Columns), "100");
    ASSERT_EQ(getMetadataString(dataset,DCM_BitsAllocated), "8");
    ASSERT_EQ(getMetadataString(dataset,DCM_BitsStored), "8");
    ASSERT_EQ(getMetadataString(dataset,DCM_HighBit), "7");
    ASSERT_EQ(getMetadataString(dataset,DCM_PixelRepresentation), "0");
    ASSERT_EQ(getMetadataString(dataset,DCM_PhotometricInterpretation), "RGB");
    ASSERT_EQ(getMetadataString(dataset,DCM_SamplesPerPixel), "3");

}

// Test 3: Verify that the first pixel (0,0) is green
TEST_F(DICOMImageTest, FirstPixelIsGreen) {

    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(testImagePath.c_str());
    ASSERT_TRUE(status.good());
    DcmDataset* dataset = fileformat.getDataset();
    
    DICOMImageTest::RGBPixel pixel = getPixelAt(dataset, 0, 0);
    ASSERT_EQ(pixel.r, 0);
    ASSERT_EQ(pixel.g, 255);
    ASSERT_EQ(pixel.b, 0);
}

// Test 4: Verify that the last pixel (99,99) is green
TEST_F(DICOMImageTest, LastPixelIsGreen) {

    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(testImagePath.c_str());
    ASSERT_TRUE(status.good());
    DcmDataset* dataset = fileformat.getDataset();
    
    DICOMImageTest::RGBPixel pixel = getPixelAt(dataset, 99, 99);
    ASSERT_EQ(pixel.r, 0);
    ASSERT_EQ(pixel.g, 255);
    ASSERT_EQ(pixel.b, 0);
}
