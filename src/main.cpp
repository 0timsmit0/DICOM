#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>

bool loadDICOMFile(const std::string& path, DcmFileFormat& fileformat)
{
    OFCondition status = fileformat.loadFile(path.c_str());
    if (status.good()) {
        std::cout << "Successfully loaded DICOM file: " << path << std::endl;
        return true;
    } else {
        std::cout << "Error loading DICOM file: " << status.text() << std::endl;
        return false;
    }
}

void printMetadata(DcmDataset* dataset) {
    DcmStack stack;
    while (dataset->nextObject(stack, OFTrue).good()) {
        DcmObject* obj = stack.top();
        DcmTag tag = obj->getTag();

        if (tag == DCM_PixelData) {
            continue;
        }

        DcmElement* elem = dynamic_cast<DcmElement*>(obj);
        if (elem) {
            OFString value;
            elem->getOFStringArray(value);
            std::cout << tag.getTagName() << ": " << value << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <dicom_file>" << std::endl;
        return 1;
    }

    const std::string path = argv[1];

    DcmFileFormat fileformat;
    if (!loadDICOMFile(path, fileformat)) {
        return 1;
    }

    DcmDataset* dataset = fileformat.getDataset();
    printMetadata(dataset);

    return 0;
}