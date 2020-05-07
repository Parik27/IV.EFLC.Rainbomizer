#include "audScriptAudioEntity.hh"
#include "Utils.hh"

void (__thiscall *audScriptAudioEntity__AddLineToConversation) (
    audScriptAudioEntity *, int, int, char *, char *, int, int);
int *        audScriptAudioEntity__m_nSoundMetadataLen;
CSoundBank **audScriptAudioEntity__g_aSoundMetadata;

/*******************************************************/
void
audScriptAudioEntity::AddLineToConversation (int index, int param_3,
                                             char *identifier, char *subtitle,
                                             int param_6, int param_7)
{
    audScriptAudioEntity__AddLineToConversation (this, index, param_3,
                                                 identifier, subtitle, param_6,
                                                 param_7);
}

/*******************************************************/
void
audScriptAudioEntity::InitialisePatternsCE ()
{
    ConvertCall (hook::get_pattern ("8b 44 ? ? 56 8b 74 ? ? 6b f6 70 03 f1 "),
                 audScriptAudioEntity__AddLineToConversation);

    char *addr = hook::get_pattern<char> (
        "55 56 ff 35 ? ? ? ? ff 35 ? ? ? ? ff b4 ? ? ? ? ? e8");

    audScriptAudioEntity__m_nSoundMetadataLen
        = injector::ReadMemory<int *> (addr + 4);

    audScriptAudioEntity__g_aSoundMetadata
        = injector::ReadMemory<CSoundBank **> (addr + 10);
}

/*******************************************************/
void
audScriptAudioEntity::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("8b 44 ? ? 6b c0 70 56 8d 34 08 8b 4c ? ?"),
                 audScriptAudioEntity__AddLineToConversation);

    char *addr
        = hook::get_pattern<char> ("8b 15 ? ? ? ? a1 ? ? ? ? 8b 4c ? ? 55 ");

    audScriptAudioEntity__m_nSoundMetadataLen
        = injector::ReadMemory<int *> (addr + 2);

    audScriptAudioEntity__g_aSoundMetadata
        = injector::ReadMemory<CSoundBank **> (addr + 7);
}

int &
audScriptAudioEntity::g_nSoundMetadataLen ()
{
    return *audScriptAudioEntity__m_nSoundMetadataLen;
}
CSoundBank *&
audScriptAudioEntity::g_aSoundMetadata ()
{
    return *audScriptAudioEntity__g_aSoundMetadata;
}
