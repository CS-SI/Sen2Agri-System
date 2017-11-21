cmake_minimum_required(VERSION 2.8)

set(GAPFILLING_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/temporalgapfilling/src/library)
set(PHENOTB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/phenotb/src/Library)
set(PHENOTB_LIBRARY OTBPheno)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestScripts/rsr_sensor_file.cfg.in
               ${CMAKE_BINARY_DIR}/rsr_sensor_file.cfg
               @ONLY)

add_subdirectory(otb-bv/src)
add_subdirectory(phenotb/src)
add_subdirectory(temporalgapfilling/src)


install(PROGRAMS    TestScripts/lai_retrieve_processing.py
                    TestScripts/lai_retrieve_processing_CS.py
                    TestScripts/pheno_processing.py
        DESTINATION bin/)
#        DESTINATION bin/python)

install(FILES TestScripts/rsr_cfg.txt
              otb-bv/data/landsat8_7b.rsr
              otb-bv/data/Sentinel2_10m.rsr
              otb-bv/data/Sentinel2_10m_real.rsr
              otb-bv/data/Sentinel2_10m_theoretical.rsr
              otb-bv/data/Sentinel2_real.rsr
              otb-bv/data/spot4hrvir1.rsr
              otb-bv/data/spot5hrg1.rsr
              otb-bv/data/spot5hrg2.rsr
              lai.map
              LaiCommonBVDistributionSamples.txt
              ${CMAKE_BINARY_DIR}/rsr_sensor_file.cfg
        DESTINATION share/sen2agri/)
