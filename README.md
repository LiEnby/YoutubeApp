# YoutubeApp
Hacked this together awhile ago- but it was never released,

there was an app on PSN that had a webbrowser in it, which has better HTML Video support than the offical one-
so i decided to see if it could load youtube, after changing user agents alot i found it works on longer videos than just 2 hours
it also lets you load videos w ads, which you cant do on youtube in browser


weirdly though, it refuses to load on Devkit, i dont know why. testkit and retail work fine thou.


ld-preload/ is LD-PRELOAD.SUPRX which i wrote some assembly code at the bottom of the EBOOT.BIN to make the app load at startup,
and forcemount/ is PRELOAD.SKPRX.
