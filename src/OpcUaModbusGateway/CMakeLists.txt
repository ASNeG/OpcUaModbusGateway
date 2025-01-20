# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# prebuild
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/Build/BuildConfig.h.in"
    "${PROJECT_BINARY_DIR}/BuildConfig.h"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/linux/etc/init/OpcUaModbusGateway.conf.in"
    "${PROJECT_BINARY_DIR}/OpcUaModbusGateway.conf"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/linux/etc/init.d/OpcUaModbusGateway.in"
    "${PROJECT_BINARY_DIR}/OpcUaModbusGateway"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/linux/usr/lib/systemd/system/OpcUaModbusGateway.service.in"
    "${PROJECT_BINARY_DIR}/OpcUaModbusGateway.service"
)

set(MODBUS_PROT_LIBRARY "${OPCUASTACK_INSTALL_PREFIX}/usr/lib/libModbusProt_shared.so")

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ${OPCUASTACK_INSTALL_PREFIX}/usr/include/ModbusProt
    ${OPCUASTACK_INSTALL_PREFIX}/usr/include/OpcUaStack${OPCUASTACK_VERSION_MAJOR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Boost_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

file(
    GLOB 
    OpcUaModbusGateway_SRC 
    ${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/*/*.cpp
)

add_library(
    OpcUaModbusGateway SHARED
    ${OpcUaModbusGateway_SRC}
    ${OPCUASTACK_INSTALL_PREFIX}/usr/lib/libModbusProt_static.a
)

target_link_libraries(
    OpcUaModbusGateway
    ${pthread}
    ${OPCUASTACK_LIBRARY_CORE}
    ${OPCUASTACK_LIBRARY_CLIENT}
    ${OPCUASTACK_LIBRARY_SERVER}
    ${OPCUASTACK_LIBRARY_PUBSUB}
    ${MODBUS_PROT_LIBRARY}
    ${Boost_LIBRARIES}
)

if (WIN32)
    target_link_libraries(OpcUaModbusGateway wsock32 ws2_32)

    if (MINGW)
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
    endif()
endif()

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# install
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

file (GLOB CONFIG_FILES ${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/Config/*.xml)
install(TARGETS OpcUaModbusGateway DESTINATION ${INSTALL_PREFIX}/lib)

if (WIN32)
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/OpcUaModbusGateway) 
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/linux/log/README DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/OpcUaModbusGateway)
    install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/OpcUaModbusGateway)
else()    
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/OpcUaModbusGateway) 
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaModbusGateway/linux/log/README DESTINATION /var/log/OpcUaStack/OpcUaModbusGateway)
    install(DIRECTORY DESTINATION /var/log/OpcUaStack/OpcUaModbusGateway)

    install(PROGRAMS ${PROJECT_BINARY_DIR}/OpcUaModbusGateway DESTINATION /etc/init.d)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaModbusGateway.conf DESTINATION /etc/init)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaModbusGateway.service DESTINATION /usr/lib/systemd/system)
endif()

if (CPACK_BINARY_DEB)
    set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/postinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/preinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/prerm"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/postrm")
endif()

if (CPACK_BINARY_RPM)
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/postinst)
    set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaModbusGateway/linux/script/prerm)
endif()
