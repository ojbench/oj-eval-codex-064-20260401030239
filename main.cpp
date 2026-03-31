#include <bits/stdc++.h>
using namespace std;

// A pragmatic runtime implementation that emulates a subset of the
// described modern C++ printf behavior for OJ interactive testing.
//
// Input format (assumed):
//  - Line 1: a format string which may contain spaces, with specifiers
//            %s, %d, %u, %%, %_
//  - Line 2: an integer N, the number of arguments
//  - Following tokens (split by whitespace) provide N arguments in order.
//    For %s the token represents the whole string without spaces.
//    For %_ default formatting:
//      * If token starts with '[' and ends with ']', it's treated as a vector
//        and printed as-is.
//      * If token is quoted with "...", quotes are stripped and content printed.
//      * If token looks like an integer with sign -> print as signed.
//      * If token looks like a non-negative integer -> print as unsigned.
//      * Otherwise print as string.

static bool is_unsigned_number(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

static bool is_signed_number(const string &s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string fmt;
    if (!std::getline(cin, fmt)) return 0;

    long long n_args = 0;
    {
        string line;
        if (!std::getline(cin, line)) line = "0";
        std::stringstream ss(line);
        ss >> n_args;
        if (!ss) n_args = 0;
    }

    vector<string> args;
    args.reserve(max(0LL, n_args));
    for (long long i = 0; i < n_args; ) {
        string tok;
        if (!(cin >> tok)) break;
        args.push_back(tok);
        ++i;
    }

    // Process format string
    ostringstream out;
    size_t ai = 0;
    for (size_t i = 0; i < fmt.size(); ++i) {
        char c = fmt[i];
        if (c != '%') {
            out << c;
            continue;
        }
        if (i + 1 >= fmt.size()) {
            // stray '%', print it literally
            out << '%';
            continue;
        }
        char sp = fmt[i + 1];
        if (sp == '%') {
            out << '%';
            ++i;
            continue;
        }

        // fetch next argument if available
        auto next_arg = [&]() -> string {
            if (ai < args.size()) return args[ai++];
            return string();
        };

        if (sp == 's') {
            string v = next_arg();
            out << v;
            ++i;
        } else if (sp == 'd') {
            string v = next_arg();
            // Fallback if not a valid number: treat as 0
            long long x = 0;
            try {
                size_t pos = 0;
                if (!v.empty()) x = stoll(v, &pos, 10);
                (void)pos;
            } catch (...) {
                x = 0;
            }
            out << x;
            ++i;
        } else if (sp == 'u') {
            string v = next_arg();
            unsigned long long x = 0;
            try {
                size_t pos = 0;
                if (!v.empty()) x = stoull(v, &pos, 10);
                (void)pos;
            } catch (...) {
                x = 0;
            }
            out << x;
            ++i;
        } else if (sp == '_') {
            string v = next_arg();
            if (v.size() >= 2 && v.front() == '[' && v.back() == ']') {
                // Vector-like token, print as-is
                out << v;
            } else if (v.size() >= 2 && v.front() == '"' && v.back() == '"') {
                // Quoted string, strip quotes
                out << v.substr(1, v.size() - 2);
            } else if (!v.empty() && v[0] == '-') {
                // Signed default -> %d
                long long x = 0;
                try { x = stoll(v); } catch (...) { x = 0; }
                out << x;
            } else if (is_unsigned_number(v)) {
                // Unsigned default -> %u
                unsigned long long x = 0;
                try { x = stoull(v); } catch (...) { x = 0; }
                out << x;
            } else {
                // Fallback to string
                out << v;
            }
            ++i;
        } else {
            // Unknown specifier, print literally
            out << '%' << sp;
            ++i;
        }
    }

    cout << out.str();
    if (!out.str().empty() && out.str().back() != '\n') cout << '\n';
    return 0;
}

