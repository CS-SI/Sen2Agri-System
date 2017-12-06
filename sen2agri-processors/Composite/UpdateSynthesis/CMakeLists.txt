otb_create_application(
  NAME           UpdateSynthesis
  SOURCES        UpdateSynthesisFunctor_2.h UpdateSynthesisFunctor_2.txx UpdateSynthesis_2.cpp
  LINK_LIBRARIES MACCSMetadata MetadataHelper ${OTB_LIBRARIES})

  otb_create_application(
  NAME           UpdateSynthesisS2
  SOURCES        UpdateSynthesisS2VectorImageFilter.h UpdateSynthesisS2VectorImageFilter.hxx UpdateSynthesisS2.cpp
  LINK_LIBRARIES ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
