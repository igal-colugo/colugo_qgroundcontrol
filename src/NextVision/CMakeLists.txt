
set(EXTRA_SRC)

add_library(NextVision
	NextVisionLinkConfiguration.cc
	NextVisionLinkConfiguration.h
	NextVisionLinkInterface.cc
	NextVisionLinkInterface.h
	NextVisionLinkManager.cc
	NextVisionLinkManager.h
	NextVisionMAVLinkProtocol.cc
	NextVisionMAVLinkProtocol.h
	NextVisionTCPLink.cc
	NextVisionTCPLink.h
	NextVisionUDPLink.cc
	NextVisionUDPLink.h

	${EXTRA_SRC}
)

target_link_libraries(NextVision
	PUBLIC
		qgc
		Qt5::Location
		Qt5::Test
		Qt5::TextToSpeech
		Qt5::Widgets
		qmdnsengine
)

target_include_directories(comm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
