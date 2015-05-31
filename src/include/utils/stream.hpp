#pragma once

#include <sstream>
#include <memory>

#include <platforms/native/types.hpp>
#include <utils/config/trace_config.hpp>
#include <utils/config/particle_system_config.hpp>
#include <CL/cl.h>

enum class StreamIgnore : unsigned char {
    NO_IGNORE    = 0x00,
    IGNORE_POS   = 0x01,
    IGNORE_VEL   = 0x02,
    IGNORE_ACCEL = 0x04,
};

inline StreamIgnore operator|(StreamIgnore lhs, StreamIgnore rhs)
{
    using T = std::underlying_type<StreamIgnore>::type;
    return (StreamIgnore)(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline StreamIgnore operator&(StreamIgnore lhs, StreamIgnore rhs)
{
    using T = std::underlying_type<StreamIgnore>::type;
    return (StreamIgnore)(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline bool operator!(StreamIgnore rhs)
{
    using T = std::underlying_type<StreamIgnore>::type;
    if (0 == static_cast<T>(rhs)) {
        return true;
    } else {
        return false;
    }
}

class ParticleStream {
public:
    ParticleStream() : m_ignore_flags(StreamIgnore::NO_IGNORE) {}
    virtual void setIgnore(StreamIgnore ignore_flags) { m_ignore_flags = ignore_flags; }
    virtual ~ParticleStream() {}

protected:
    StreamIgnore m_ignore_flags;
};

class ParticleOStream : public ParticleStream {
public:
    virtual void open(std::string filename) = 0;
    virtual bool good() = 0;

    virtual void Write(md::float3 pos, md::float3 vel, md::float3 accel) = 0;
    virtual void Write(cl_float3 pos, cl_float3 vel, cl_float3 accel) = 0;
};

using ParticleOStreamPtr = std::shared_ptr<ParticleOStream>;

class ByteOStream : public ParticleOStream {
public:
    ByteOStream();
    ByteOStream(std::string filename);
    ByteOStream(std::ostream& os);

    virtual void open(std::string filename);
    virtual bool good();

    virtual void Write(md::float3 pos, md::float3 vel, md::float3 accel);
    virtual void Write(cl_float3 pos, cl_float3 vel, cl_float3 accel);

protected:
    std::shared_ptr<std::ostream> m_stream_ptr;
};

class TextOStream : public ParticleOStream {
public:
    TextOStream();
    TextOStream(std::string filename);
    TextOStream(std::ostream& os);

    virtual void open(std::string filename);
    virtual bool good();

    virtual void Write(md::float3 pos, md::float3 vel, md::float3 accel);
    virtual void Write(cl_float3 pos, cl_float3 vel, cl_float3 accel);

protected:
    std::shared_ptr<std::ostream> m_stream_ptr;
};


class ParticleIStream : public ParticleStream {
public:
    virtual void open(std::string filename) = 0;
    virtual bool good() = 0;

    virtual void Read(md::float3& pos, md::float3& vel, md::float3& accel) = 0;
    virtual void Read(cl_float3& pos, cl_float3& vel, cl_float3& accel) = 0;
};

using ParticleIStreamPtr = std::shared_ptr<ParticleIStream>;

class ByteIStream : public ParticleIStream {
public:
    ByteIStream();
    ByteIStream(std::string filename);
    ByteIStream(std::istream& os);

    virtual void open(std::string filename);
    virtual bool good();

    virtual void Read(md::float3& pos, md::float3& vel, md::float3& accel);
    virtual void Read(cl_float3& pos, cl_float3& vel, cl_float3& accel);

protected:
    std::shared_ptr<std::istream> m_stream_ptr;
};

class TextIStream : public ParticleIStream {
public:
    TextIStream();
    TextIStream(std::string filename);
    TextIStream(std::istream& os);

    virtual void open(std::string filename);
    virtual bool good();

    virtual void Read(md::float3& pos, md::float3& vel, md::float3& accel);
    virtual void Read(cl_float3& pos, cl_float3& vel, cl_float3& accel);

protected:
    std::shared_ptr<std::istream> m_stream_ptr;
};

class StringStream : public TextOStream {
public:
    StringStream();
    std::string str();
    std::stringstream& stream() { return m_ss; }

protected:
    std::stringstream m_ss;
};

using StringStreamPtr = std::shared_ptr<StringStream>;

class StreamFactory {
public:
    static StreamFactory* Instance()
    {
        static StreamFactory sf;
        return &sf;
    }

    ParticleIStreamPtr MakeTraceIStream(TraceConfig conf);
    ParticleOStreamPtr MakeTraceOStream(TraceConfig conf);
    ParticleIStreamPtr MakeInitIStream(ParticleSystemConfig conf);
    ParticleOStreamPtr MakeResultOStream(ParticleSystemConfig conf);

protected:
    StreamFactory()
    {
    }
};
