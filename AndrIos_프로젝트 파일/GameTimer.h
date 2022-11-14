#pragma once

// ½Ã°£
time_t t = time(NULL);
struct tm tm = *localtime(&t);

static unsigned __int64 GameTimer{ 0 };

clock_t CurTime, OldTime;