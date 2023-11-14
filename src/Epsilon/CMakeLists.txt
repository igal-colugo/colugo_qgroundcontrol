
set(EXTRA_SRC)

add_library(Epsilon
	EpsilonLinkConfiguration.cc
	EpsilonLinkConfiguration.h
	EpsilonLinkInterface.cc
	EpsilonLinkInterface.h
	EpsilonLinkManager.cc
	EpsilonLinkManager.h
	EpsilonMAVLinkProtocol.cc
	EpsilonMAVLinkProtocol.h
	EpsilonLinkProtocol.cc
	EpsilonLinkProtocol.h
	EpsilonTCPLink.cc
	EpsilonTCPLink.h
	EpsilonUDPLink.cc
	EpsilonUDPLink.h

	${EXTRA_SRC}
)

target_link_libraries(Epsilon
	PUBLIC
		qgc
		Qt5::Location
		Qt5::Test
		Qt5::TextToSpeech
		Qt5::Widgets
		qmdnsengine
)

target_include_directories(comm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
