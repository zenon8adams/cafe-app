#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <QDebug>
#include "rsa-encryption.h"

namespace cafe {
namespace utility {

class RSAEncryption::Implementation
{
    struct Num
    {
        intmax_t div = 0,
                quo = 0,
                rem = 0,
                value = 0;
        friend bool operator==(const Num& l, const Num& r)
        {
            return l.div == r.div && l.quo == r.quo && l.rem == r.rem && l.value == r.value;
        }
        friend bool operator!=(const Num& l, const Num& r)
        {
            return !(l == r);
        }
    };
    intmax_t prime1, prime2, product, totientFn, pub_key, priv_key;
    std::vector<Num> euclid_buffer_;
    static const std::string nulls_,
                             cipher_sep_;
    static const     char    key_sep_;

    RSAEncryption *rsaEncryption{ nullptr };
public:
    Implementation(RSAEncryption *_rsaEncryption, intmax_t _prime1, intmax_t _prime2)
        : prime1(_prime1),
          prime2(_prime2),
          product(prime1 * prime2),
          totientFn{(prime1-1) * (prime2-1)},
          rsaEncryption(_rsaEncryption)
    {
        srand(time(nullptr));
        auto coprimes = coprimesOf(totientFn);
        pub_key = coprimes[ rand() % coprimes.size() ];
        generatePrivateKey(pub_key);
        Q_UNUSED(rsaEncryption)
    }
    void generatePrivateKey(int64_t public_key)
    {
        euclid_buffer_.clear();
        auto result = euclideanAlgorithm(totientFn, public_key);
        priv_key = extendedEuclideanAlgorithm(result);
        if(priv_key < 0)
            priv_key =  (totientFn + priv_key) % totientFn;
    }
    std::vector<intmax_t> primesOf(intmax_t n)
    {
        std::vector<bool> v(n+1, true);
        v[0] = v[1] = false;
        for(int i = 2; i <= n; ++i)
        {
            for(int j = 2; (j * i) <= n; ++j)
                v[j * i] = false;
        }
        std::vector<intmax_t> primes;
        for(int i = 0; i < v.size()-1; ++i) {
            if(v[i])
            primes.push_back(i);
        }
        return primes;
    }
    int isPrime(intmax_t n)
    {
        int i  = 2;
        while(i <= n)
        {
            int trialDiv = 2;
            while(trialDiv * trialDiv <= i)
            {
                if(i % trialDiv == 0)
                {
                    if(i == n)
                        return false;
                    break;
                }
                ++trialDiv;
            }
            ++i;
        }
        return true;
    }
    std::vector<Num> euclideanAlgorithm(intmax_t totFn, intmax_t pubKey)
    {
        Num line;
        if(pubKey == 1 || pubKey == 0)
            return euclid_buffer_;
        line.rem = totFn % pubKey;
        line.quo = totFn / (line.div = pubKey);
        line.value = line.quo * line.div + line.rem;
        line.quo *= -1;
        euclid_buffer_.push_back(line);
        return euclideanAlgorithm(line.div, line.rem);
    }
    bool do_expr_1(Num& exp1, Num& exp2, Num& exp3)
    {
        if (exp1.quo == exp2.div) {
            exp1 = {exp2.div, exp1.div + exp2.quo};
            exp2 = exp3;
        }else if(exp1.quo == exp3.div)
            exp1 = {exp3.div, exp1.div + exp3.quo};
        exp3 = {};
        return true;
    }
    bool do_expr(Num& exp1, Num& exp2, Num& exp3)
    {
        if (exp1.div == exp2.div) {
            exp1 = {exp1.div, exp1.quo + exp2.quo};
            exp2 = exp3;
        }else if(exp2.div == exp3.div)
        {
            auto tmp = exp1;
            exp1 = {exp2.div, exp2.quo + exp3.quo};
            exp2 = tmp;
        }else if(exp1.div == exp3.div)
            exp1 = {exp1.div, exp1.quo + exp3.quo};
        exp3 = {};
        return true;
    }
    intmax_t extendedEuclideanAlgorithm(std::vector<Num>& toks)
    {
        if(toks.size() == 1)
            return toks[0].quo;
        Num lhs, rhs;
        for(auto begin = toks.crbegin(), end = --toks.crend(); begin != end; ++begin)
        {
            Num x, y, exp1, exp2, exp3;
            if(lhs != Num{} && rhs != Num{})
            {
                x = lhs;
                y = rhs;
                if(x.div == std::next(begin)->rem)
                {
                    exp1 = {std::next(begin)->value, x.quo};
                    exp2 = {std::next(begin)->div, std::next(begin)->quo * x.quo};
                    exp3 = y;
                }else if(y.div == std::next(begin)->rem)
                {
                    exp1 = x;
                    exp2 = {std::next(begin)->value, y.quo};
                    exp3 = {std::next(begin)->div, y.quo * std::next(begin)->quo};
                }
            }else {
                x = {1, begin->value};
                y = {begin->div, begin->quo};
                if(x.div == std::next(begin)->rem)
                {
                    exp1 = {std::next(begin)->value, begin->value};
                    exp2 = {std::next(begin)->div, std::next(begin)->quo * begin->value};
                    exp3 = y;
                }else if(y.div == std::next(begin)->rem)
                {
                    exp1 = x;
                    exp2 = {std::next(begin)->value, y.quo};
                    exp3 = {std::next(begin)->div, y.quo * std::next(begin)->quo};
                }
            }

            if(exp1.div == 1)
                do_expr_1(exp1, exp2, exp3);
            else if(exp2.div == 1)
                do_expr_1(exp2, exp1, exp3);
            else if(exp3.div == 1)
                do_expr_1(exp3, exp1, exp3);
            else do_expr(exp1, exp2, exp3);
            lhs = exp1;
            rhs = exp2;
        }
        return rhs.quo == pub_key ? rhs.quo : lhs.quo;
    }
    std::vector<intmax_t> coprimesOf(intmax_t num)
    {
        auto primes = primesOf(num);
        std::vector<intmax_t> coprimes;
        for(const auto& elm : primes)
        {
            if(num % elm != 0)
            {
                coprimes.push_back(elm);
                int prod = elm * elm;
                while(prod < num)
                {
                    coprimes.push_back(prod);
                    prod *= elm;
                }
            }
        }
        std::sort(coprimes.begin(), coprimes.end());
        return coprimes;
    }
    intmax_t mod(intmax_t quo, intmax_t idx, intmax_t div)
    {
        intmax_t quo_2 = quo % div,
        rdx = 1;
        for(int i = 1; i <= idx; ++i)
        {
            rdx *= quo_2;
            rdx = (rdx % div) % div;
        }
        return rdx;
    }
    std::vector<std::string> split(std::string str, char c)
    {
        std::vector<std::string> substr;
        size_t idx;
        while((idx = str.find(c)) != std::string::npos && idx < str.size())
        {
            substr.emplace_back( std::string(str.cbegin(), str.cbegin()+idx) );
            str = str.substr(idx+1);
        }
        if(!str.empty())
            substr.push_back(str);
        return substr;
    }
    std::vector<std::string> split(const std::string& str)
    {
        std::string tmp;
        std::vector<std::string> substr;
        for(const auto& elm : str)
        {
            if(cipher_sep_.find(elm) != std::string::npos)
            {
                if(tmp.empty())
                    continue;
                substr.push_back(tmp);
                tmp.clear();
            }else tmp += elm;
        }
        if(!tmp.empty())
            substr.push_back(tmp);
        return substr;
    }
    std::string digits(int stmt)
    {
        return std::to_string(std::abs(stmt));
    }
    std::string extractKey(std::string& cipher)
    {
        std::string tkey, key;
        auto pos = cipher.find(key_sep_);
        if(pos != std::string::npos)
        {
            size_t nextpos = pos+1;
            for(; nextpos < cipher.size() && cipher_sep_.find(cipher[nextpos]) == std::string::npos; ++nextpos)
                tkey += cipher[nextpos];
            cipher = cipher.substr(0, pos) + cipher.substr(nextpos+1);
            for(const auto& elm : tkey)
            {
                if(nulls_.find(elm) == std::string::npos)
                    key += elm;
            }
            for(auto& elm : key)
                elm = char(elm + '0' - 'a');
            return key;
        }
        return {};
    }
    void setPublicKey(const std::string& rep)
    {
        if(!rep.empty())
        {
            pub_key = std::stol(rep);
            generatePrivateKey(pub_key);
        }
    }
    std::string encrypt(const std::string& plaintext, int nulls = 0, bool append_key = false)
    {
        std::string cipher;
        for(size_t i = 0; i < plaintext.size(); ++i)
        {
            int cnum = mod(plaintext[i] - 'a', pub_key, product);
            if(cnum < 0)
                cipher += '_';
            for(const auto& elm : digits(cnum))
                cipher += char(elm - '0' + 'a');
            if(i != plaintext.size()-1)
                cipher += cipher_sep_[ rand() % cipher_sep_.size()];
        }
        if(std::abs(nulls))
        {
            const int null_count = std::abs(nulls);
            for(int i = 0; i < null_count; ++i)
                cipher.insert(rand() % cipher.size(), 1, nulls_[ rand() % nulls_.size() ]);
        }
        if(append_key)
        {
            cipher.insert(rand() % cipher.size(), 1, key_sep_);
            auto pos = cipher.find(key_sep_);
            cipher.insert(pos+1, [=]{ std::string s = digits(pub_key), ret_s;
                                      std::for_each(s.cbegin(), s.cend(), [&ret_s](const auto& elm){ ret_s += char(elm - '0' + 'a'); });
                                      return ret_s  + cipher_sep_[rand() % cipher_sep_.size()]; }());

        }

        return cipher;
    }
    std::string decrypt(std::string ctext)
    {
        std::string nctext, decipher, key = extractKey(ctext);
        for(const auto& elm : ctext)
        {
            if(nulls_.find(elm) == std::string::npos)
                nctext += elm;
        }
        ctext.clear();
        auto toks = split(nctext);
        for(const auto& elm : toks)
        {
            for(const auto& mem : elm)
            {
                if(mem == '_') ctext += '-';
                else ctext += char(mem - 'a' + '0');
            }
            if(&elm != &toks[toks.size()-1])
                ctext += ',';
        }
        toks = split(ctext, ',');
        if(!key.empty())
            generatePrivateKey(std::stol(key));
        for(const auto& elm : toks)
        {
            char c  = mod(std::stol(elm), priv_key, product);
            decipher += char(c + 'a');
        }
        return decipher;
    }
};
const std::string RSAEncryption::Implementation::nulls_ = {"0123456789ABCDEFGHIJKLqrtuvwxyz"},
                  RSAEncryption::Implementation::cipher_sep_ = {"MNOPQRSTUVWXYZklmnop"};
const     char    RSAEncryption::Implementation::key_sep_ {'s'};

RSAEncryption::RSAEncryption(const std::pair<intmax_t, intmax_t>& seed, QObject *parent)
    : QObject(parent)
{
     implementation.reset(new Implementation(this, seed.first, seed.second));
}
RSAEncryption::~RSAEncryption()
{
}
QString RSAEncryption::encrypt(QString plaintext, int nulls, bool append_key)
{
    return QString::fromStdString(implementation->encrypt(plaintext.toStdString(), nulls, append_key));
}
QString RSAEncryption::decrypt(QString cipheredtext)
{
    return QString::fromStdString(implementation->decrypt(cipheredtext.toStdString()));
}

QString RSAEncryption::extractKey(QString& cipheredtext)
{
    std::string tmp{cipheredtext.toStdString()};
     cipheredtext = QString::fromStdString(implementation->extractKey(tmp));
    return cipheredtext;
}

void RSAEncryption::setPublicKey(const QString &rep)
{
    implementation->setPublicKey(rep.toStdString());
}

}
}
