#include "word_break.h"

#include <catch2/catch.hpp>

TEST_CASE("benchmark test") {
    auto const english_lexicon = ::word_break::read_lexicon("./english.txt");
    auto const sentences = ::word_break::word_break("dogsandragonflytobeornotobethatisthequestionstudentsstudyprogrammingtodaydogsandragonflytobeornotobethatisthequestionstudentsstudyprogrammingtodaybirdssingbeautifulmelodiescatandogruntimeandtimeagainseethesunrisethereisnoplacehomewhatimeisitanicedaynotevenonceletmegooutinthenameofgodgoingtowashingtonseaandlandhotandcoldbirdssingbeautifulmelodiescatandogruntimeandtimeagainseethesunrisethereisnoplacehomewhatimeisitanicedaynotevenonceletmegooutinthenameofgodgoingtowashingtonseaandlandhotandcold", english_lexicon);

    CHECK(std::size(sentences) != 0);
}
