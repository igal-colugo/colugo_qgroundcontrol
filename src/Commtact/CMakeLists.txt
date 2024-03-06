
set(EXTRA_SRC)

add_library(Commtact
      	CommtactLinkManagement.cc
      	CommtactLinkManagement.h
	CommtactLinkConfiguration.cc
	CommtactLinkConfiguration.h
	CommtactLinkInterface.cc
	CommtactLinkInterface.h
	CommtactLinkManager.cc
	CommtactLinkManager.h
	CommtactLinkProtocol.cc
	CommtactLinkProtocol.h
	CommtactTCPLink.cc
	CommtactTCPLink.h
	CommtactUDPLink.cc
	CommtactUDPLink.h

	${EXTRA_SRC}
)

target_link_libraries(Commtact
	PUBLIC
		qgc
		Qt5::Location
		Qt5::Test
		Qt5::TextToSpeech
		Qt5::Widgets
		qmdnsengine
)

target_include_directories(comm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
