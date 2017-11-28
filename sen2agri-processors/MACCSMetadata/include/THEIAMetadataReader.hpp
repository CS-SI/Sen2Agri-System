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
 
#pragma once

#include <memory>

#include "itkObjectFactory.h"
#include "otb_tinyxml.h"

#include "THEIAMetadata.hpp"

namespace itk
{
class THEIAMetadataReader : public itk::LightObject
{
public:
    typedef THEIAMetadataReader Self;
    typedef itk::LightObject Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

public:
    itkNewMacro(Self)

    itkTypeMacro(THEIAMetadataReader, itk::LightObject)

    std::unique_ptr<THEIAMetadata> ReadMetadata(const std::string &path);

private:
    std::unique_ptr<THEIAMetadata> ReadMetadataXml(const TiXmlDocument &doc);
};
}

std::string dirname(const std::string &path);
std::string basename(const std::string &path);
