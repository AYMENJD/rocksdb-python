#pragma once

#include <exception>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <rocksdb/db.h>

namespace py = pybind11;
using status = rocksdb::Status;

class Response {
   public:
    rocksdb::Status status;
    std::string value;
    rocksdb::Options options;

    Response(rocksdb::Status &s, std::string *value = nullptr, rocksdb::Options *options = nullptr) {
        if (value != nullptr) {
            this->value = *value;
        }
        if (options != nullptr) {
            this->options = *options;
        }

        this->status = s;
    }
};

class RocksDB {
   public:
    rocksdb::DB *db;
    bool is_running = false;
    bool read_only = false;

    RocksDB(const std::string db_path, rocksdb::Options &op, bool read_only = false,
            std::string *secondary_path = nullptr) {
        status s;

        if (read_only == false) {
            s = rocksdb::DB::Open(op, db_path, &this->db);
        } else if (read_only == true) {
            if (secondary_path == nullptr) {
                throw std::invalid_argument("secondary_path must be non-empty");
            } else {
                s = rocksdb::DB::OpenAsSecondary(op, db_path, *secondary_path, &this->db);
            }

        } else {
            throw std::invalid_argument("read_only must be boolean");
        }

        if (!s.ok()) {
            throw std::runtime_error(s.getState());
        }

        this->is_running = true;
        this->read_only = read_only;
    }

    Response Get(rocksdb::ReadOptions &options, std::string &key) {
        CHECK_DB();

        status s;
        std::string value;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else {
            s = this->db->Get(options, key, &value);
        }

        return Response(s, &value);
    }

    Response Put(rocksdb::WriteOptions &options, std::string &key, std::string &value) {
        CHECK_DB();

        status s;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else if (value.empty()) {
            s = status::InvalidArgument("Value must be non-empty");
        } else {
            s = this->db->Put(options, key, value);
        }

        return Response(s);
    }

    Response Merge(rocksdb::WriteOptions &options, std::string &key, std::string &value) {
        CHECK_DB();

        status s;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else if (value.empty()) {
            s = status::InvalidArgument("Value must be non-empty");
        } else {
            s = this->db->Merge(options, key, value);
        }

        return Response(s, &value);
    }

    Response KeyMayExist(rocksdb::ReadOptions &options, std::string &key) {
        CHECK_DB();

        status s;
        std::string value;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else {
            bool found = this->db->KeyMayExist(options, key, &value);
            s = found ? status::OK() : status::NotFound();
        }

        return Response(s, &value);
    }

    Response Del(rocksdb::WriteOptions &options, std::string &key) {
        CHECK_DB();

        status s;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else {
            s = this->db->Delete(options, key);
        }

        return Response(s);
    }

    Response GetOptions() {
        CHECK_DB();

        rocksdb::Options op = this->db->GetOptions();
        status s = status::OK();
        return Response(s, nullptr, &op);
    }

    Response SetOptions(py::dict &dict_options) {
        CHECK_DB();

        status s;

        std::unordered_map<std::string, std::string> options;
        for (std::pair<py::handle, py::handle> item : dict_options) {
            options[item.first.cast<std::string>()] = item.second.cast<std::string>();
        }

        s = this->db->SetOptions(options);

        return Response(s);
    }

    Response SetDBOptions(py::dict &dict_options) {
        CHECK_DB();

        status s;

        std::unordered_map<std::string, std::string> options;
        for (std::pair<py::handle, py::handle> item : dict_options) {
            options[item.first.cast<std::string>()] = item.second.cast<std::string>();
        }

        s = this->db->SetDBOptions(options);

        return Response(s);
    }

    Response GetProperty(std::string &key) {
        CHECK_DB();

        status s;
        std::string value;

        if (key.empty()) {
            s = status::InvalidArgument("Key must be non-empty");
        } else {
            bool found = this->db->GetProperty(key, &value);
            s = found ? status::OK() : status::NotFound("Property '" + key + "' not found");
        }

        return Response(s, &value);
    }

    Response Flush(rocksdb::FlushOptions &options) {
        CHECK_DB();

        status s = this->db->Flush(options);
        return Response(s);
    }

    Response TryCatchUpWithPrimary() {
        CHECK_DB();

        status s;

        if (this->read_only == false) {
            s = status::NotSupported("TryCatchUpWithPrimary is not supported for non-secondary instance");
        } else {
            s = this->db->TryCatchUpWithPrimary();
        }

        return Response(s);
    }

    static std::unordered_map<std::string, std::string> GetRocksBuildProperties() {
        return ROCKSDB_NAMESPACE::GetRocksBuildProperties();
    }

    static std::string GetRocksVersionAsString() {
        return ROCKSDB_NAMESPACE::GetRocksVersionAsString();
    }

    static std::string GetRocksBuildInfoAsString(std::string &name, bool &verbose) {
        return ROCKSDB_NAMESPACE::GetRocksBuildInfoAsString(name, verbose);
    }

    Response Close() {
        if (this->is_running) {
            this->is_running = false;
            status s = this->db->Close();
            return Response(s);
        } else {
            throw std::runtime_error("Database already closed");
        }
    }

   private:
    void CHECK_DB() {
        if (!this->is_running) {
            throw std::runtime_error("Cannot invoke request database closed");
        }
    }
};
