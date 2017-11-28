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


std::string ReadFixedHeader(const TiXmlElement *el)
{
    std::string result;

    if (!el) {
        return result;
    }

    result = GetChildText(el, "AUTHORITY");
    
    return result;
}


std::unique_ptr<THEIAMetadata> THEIAMetadataReader::ReadMetadataXml(const TiXmlDocument &doc)
{
    TiXmlHandle hDoc(const_cast<TiXmlDocument *>(&doc));
    
    auto rootElement = hDoc.FirstChildElement("Muscate_Metadata_Document").ToElement();

    if (!rootElement) {
        return nullptr;
    }


    auto file = std::unique_ptr<THEIAMetadata>(new THEIAMetadata);
    
    file->MissionName = ReadFixedHeader(rootElement->FirstChildElement("Dataset_Identification"));
    
    return file;
}

}
