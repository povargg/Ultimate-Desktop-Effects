#include "../src/update_release.h"
#include <cassert>
#include <iostream>

int main() {
    using updates::ParseManifest;
    using updates::ParseVersion;

    assert(ParseVersion("1.10.0") > ParseVersion("1.9.99"));
    assert(ParseVersion("2.0.0") > ParseVersion("1.99.99"));
    for (const auto version : {"", "v1.0.3", "1.2", "1.2.3.4", "1..3", "1.2.03", "-1.2.3", "1.2.3-beta", "9999999.0.0"}) {
        assert(!ParseVersion(version));
    }

    const std::string hash = "087B84985A8D51831D39E7F80BD11D814BF7022FA77B23249BE18030A61C71BE";
    const auto release = ParseManifest("1.0.3\r\n" + hash + "\r\n");
    assert(release && release->tag == "1.0.3");
    assert(release->sha256 == "087b84985a8d51831d39e7f80bd11d814bf7022fa77b23249be18030a61c71be");
    assert(!ParseManifest("1.0.3\n" + hash + "\nextra"));
    assert(!ParseManifest("1.0.3\n" + hash.substr(1)));
    assert(!ParseManifest("1.0.3\n?" + hash.substr(1)));
    assert(!ParseManifest("1.0.3"));
    assert(!ParseManifest("https://example.invalid/\n" + hash));
    std::cout << "Release manifest and version checks passed.\n";
}
