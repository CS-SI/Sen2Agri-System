/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2017, CS SI
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/
 
#include "THEIAMetadataReader.hpp"
#include "tinyxml_utils.hpp"


namespace itk
{
std::unique_ptr<THEIAMetadata> THEIAMetadataReader::ReadMetadata(const std::string &path)
{
    TiXmlDocument doc(path);
    if (!doc.LoadFile()) {
        return nullptr;
    }

    auto metadata = this->ReadMetadataXml(doc);

    return metadata;
}

THEIADatasetIdentification ReadDatasetIdentification(const TiXmlElement *el)
{
    THEIADatasetIdentification result;

    if (!el) {
        return result;
    }

    result.Authority = GetChildText(el, "AUTHORITY");
    result.Project = GetChildText(el, "PROJECT");
    result.Producer = GetChildText(el, "PRODUCER");

    return result;
}

std::map<std::string, THEIAMask> ReadMasks(const TiXmlElement *el)
{
    std::map<std::string, THEIAMask> result;

    if (!el) {
        return result;
    }

    for (   auto maskEl = el->FirstChildElement("Mask");
            maskEl;
            maskEl = maskEl->NextSiblingElement("Mask")) {

        auto maskProperties = maskEl->FirstChildElement("Mask_Properties");

        std::string nature = GetChildText(maskProperties, "NATURE");
        //std::cout << "Nature: " << nature << std::endl;

        auto maskListEl = maskEl->FirstChildElement("Mask_File_List");

        THEIAMask masks;

        for (   auto maskFileEl = maskListEl->FirstChildElement("MASK_FILE");
                maskFileEl;
                maskFileEl = maskFileEl->NextSiblingElement("MASK_FILE")) {
            std::string maskFile = maskFileEl->GetText();
            //std::cout << maskFile << std::endl;
            std::string groupId;
            maskFileEl->QueryValueAttribute("group_id", &groupId);
            //std::cout << groupId << std::endl;

            masks[groupId] = maskFile;
        }
        result[nature] = masks;
    }

    //for ( const auto &pair : result) {
        //std::cout << pair.first << ": [ " ;
        //for ( const auto &pair2 : pair.second) {
            //std::cout << pair2.first << ":" << pair2.second << ", ";
        //}
        //std::cout << "]" << std::endl;
    //}

    return result;
}

std::map<std::string, THEIAImg> ReadImages(const TiXmlElement *el)
{
    std::map<std::string, THEIAImg> result;

    if (!el) {
        return result;
    }

    for (   auto imageEl = el->FirstChildElement("Image");
            imageEl;
            imageEl = imageEl->NextSiblingElement("Image")) {

        auto maskProperties = imageEl->FirstChildElement("Image_Properties");

        std::string nature = GetChildText(maskProperties, "NATURE");

        auto imgListEl = imageEl->FirstChildElement("Image_File_List");

        THEIAImg images;

        for (   auto imgFileEl = imgListEl->FirstChildElement("IMAGE_FILE");
                imgFileEl;
                imgFileEl = imgFileEl->NextSiblingElement("IMAGE_FILE")) {
            std::string imageFile = imgFileEl->GetText();
            if (nature.compare("Aerosol_Optical_Thickness") == 0  || nature.compare("Water_Vapor_Content") == 0 ) {
                std::string groupId;
                imgFileEl->QueryValueAttribute("group_id", &groupId);
                images[groupId] = imageFile;
            }
            else {
                std::string bandId;
                imgFileEl->QueryValueAttribute("band_id", &bandId);
                images[bandId] = imageFile;
            }
        }
        result[nature] = images;
    }

    //for ( const auto &pair : result) {
        //std::cout << pair.first << ": [ " ;
        //for ( const auto &pair2 : pair.second) {
            //std::cout << pair2.first << ":" << pair2.second << ", ";
        //}
        //std::cout << "]" << std::endl;
    //}

    return result;
}

THEIAProductOrganisation ReadProductOrganisation(const TiXmlElement *el)
{
    std::cout << "ReadProductOrganisation" << std::endl;

    THEIAProductOrganisation result;

    if (!el) {
        return result;
    }

    auto muscateEl = el->FirstChildElement("Muscate_Product");

    result.QLK = GetChildText(muscateEl, "QUICKLOOK");

    result.IMGS = ReadImages(muscateEl->FirstChildElement("Image_List"));

    result.MASKS = ReadMasks(muscateEl->FirstChildElement("Mask_List"));

    return result;
}

std::unique_ptr<THEIAMetadata> THEIAMetadataReader::ReadMetadataXml(const TiXmlDocument &doc)
{
    TiXmlHandle hDoc(const_cast<TiXmlDocument *>(&doc));

    auto rootElement = hDoc.FirstChildElement("Muscate_Metadata_Document").ToElement();

    if (!rootElement) {
        return nullptr;
    }

    auto theiaMetadata = std::unique_ptr<THEIAMetadata>(new THEIAMetadata);

    theiaMetadata->datasetIdentification = ReadDatasetIdentification(rootElement->FirstChildElement("Dataset_Identification"));

    theiaMetadata->productOrganisation = ReadProductOrganisation(rootElement->FirstChildElement("Product_Organisation"));

    return theiaMetadata;
}

}
