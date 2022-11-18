#include "rocksdb.hpp"
using namespace py::literals;

PYBIND11_MODULE(rocksdb_ext, m) {
    py::class_<RocksDB>(m, "RocksDBext")
        .def(py::init<std::string, rocksdb::Options &, bool, std::string *>())
        .def_readonly("is_running", &RocksDB::is_running)
        .def("Get", &RocksDB::Get, py::arg("readOptions"), py::arg("key"), py::return_value_policy::move)
        .def("Put", &RocksDB::Put, py::arg("writeOptions"), py::arg("key"), py::arg("value"),
             py::return_value_policy::move)
        .def("Merge", &RocksDB::Merge, py::arg("writeOptions"), py::arg("key"), py::arg("value"),
             py::return_value_policy::move)
        .def("KeyMayExist", &RocksDB::KeyMayExist, py::arg("readOptions"), py::arg("key"),
             py::return_value_policy::move)
        .def("Del", &RocksDB::Del, py::arg("writeOptions"), py::arg("key"), py::return_value_policy::move)
        .def("GetOptions", &RocksDB::GetOptions, py::return_value_policy::move)
        .def("SetOptions", &RocksDB::SetOptions, py::arg("options"), py::return_value_policy::move)
        .def("SetDBOptions", &RocksDB::SetDBOptions, py::arg("options"), py::return_value_policy::move)
        .def("GetProperty", &RocksDB::GetProperty, py::arg("key"), py::return_value_policy::move)
        .def("Flush", &RocksDB::Flush, py::arg("flushOptions"), py::return_value_policy::move)
        .def("TryCatchUpWithPrimary", &RocksDB::TryCatchUpWithPrimary, py::return_value_policy::move)
        .def_static("GetRocksBuildProperties", &RocksDB::GetRocksBuildProperties)
        .def_static("GetRocksVersionAsString", &RocksDB::GetRocksVersionAsString, py::return_value_policy::move)
        .def_static("GetRocksBuildInfoAsString", &RocksDB::GetRocksBuildInfoAsString, py::return_value_policy::move)
        .def("Close", &RocksDB::Close, py::return_value_policy::move);

    py::class_<Response>(m, "Response")
        .def(py::init<rocksdb::Status &, std::string *>())
        .def_readwrite("status", &Response::status)
        .def_readwrite("options", &Response::options)
        .def_readwrite("value", &Response::value);

    // RocksDB options aka rocksdb::Options
    py::class_<rocksdb::Options>(m, "_Options")
        .def(py::init())
        .def_readwrite("write_buffer_size", &rocksdb::Options::write_buffer_size)
        .def_readwrite("level0_file_num_compaction_trigger", &rocksdb::Options::level0_file_num_compaction_trigger)
        .def_readwrite("max_bytes_for_level_base", &rocksdb::Options::max_bytes_for_level_base)
        .def_readwrite("disable_auto_compactions", &rocksdb::Options::disable_auto_compactions)
        .def_readwrite("create_if_missing", &rocksdb::Options::create_if_missing)
        .def_readwrite("create_missing_column_families", &rocksdb::Options::create_missing_column_families)
        .def_readwrite("error_if_exists", &rocksdb::Options::error_if_exists)
        .def_readwrite("paranoid_checks", &rocksdb::Options::paranoid_checks)
        .def_readwrite("flush_verify_memtable_count", &rocksdb::Options::flush_verify_memtable_count)
        .def_readwrite("track_and_verify_wals_in_manifest", &rocksdb::Options::track_and_verify_wals_in_manifest)
        .def_readwrite("verify_sst_unique_id_in_manifest", &rocksdb::Options::verify_sst_unique_id_in_manifest)
        .def_readwrite("max_open_files", &rocksdb::Options::max_open_files)
        .def_readwrite("max_file_opening_threads", &rocksdb::Options::max_file_opening_threads)
        .def_readwrite("max_total_wal_size", &rocksdb::Options::max_total_wal_size)
        .def_readwrite("use_fsync", &rocksdb::Options::use_fsync)
        .def_readwrite("db_log_dir", &rocksdb::Options::db_log_dir)
        .def_readwrite("wal_dir", &rocksdb::Options::wal_dir)
        .def_readwrite("delete_obsolete_files_period_micros", &rocksdb::Options::delete_obsolete_files_period_micros)
        .def_readwrite("max_background_jobs", &rocksdb::Options::max_background_jobs)
        .def_readwrite("max_background_compactions", &rocksdb::Options::max_background_compactions)
        .def_readwrite("max_subcompactions", &rocksdb::Options::max_subcompactions)
        .def_readwrite("max_background_flushes", &rocksdb::Options::max_background_flushes)
        .def_readwrite("max_log_file_size", &rocksdb::Options::max_log_file_size)
        .def_readwrite("log_file_time_to_roll", &rocksdb::Options::log_file_time_to_roll)
        .def_readwrite("keep_log_file_num", &rocksdb::Options::keep_log_file_num)
        .def_readwrite("recycle_log_file_num", &rocksdb::Options::recycle_log_file_num)
        .def_readwrite("max_manifest_file_size", &rocksdb::Options::max_manifest_file_size)
        .def_readwrite("table_cache_numshardbits", &rocksdb::Options::table_cache_numshardbits)
        .def_readwrite("WAL_ttl_seconds", &rocksdb::Options::WAL_ttl_seconds)
        .def_readwrite("WAL_size_limit_MB", &rocksdb::Options::WAL_size_limit_MB)
        .def_readwrite("manifest_preallocation_size", &rocksdb::Options::manifest_preallocation_size)
        .def_readwrite("allow_mmap_reads", &rocksdb::Options::allow_mmap_reads)
        .def_readwrite("allow_mmap_writes", &rocksdb::Options::allow_mmap_writes)
        .def_readwrite("use_direct_reads", &rocksdb::Options::use_direct_reads)
        .def_readwrite("use_direct_io_for_flush_and_compaction",
                       &rocksdb::Options::use_direct_io_for_flush_and_compaction)
        .def_readwrite("allow_fallocate", &rocksdb::Options::allow_fallocate)
        .def_readwrite("is_fd_close_on_exec", &rocksdb::Options::is_fd_close_on_exec)
        .def_readwrite("stats_dump_period_sec", &rocksdb::Options::stats_dump_period_sec)
        .def_readwrite("stats_persist_period_sec", &rocksdb::Options::stats_persist_period_sec)
        .def_readwrite("persist_stats_to_disk", &rocksdb::Options::persist_stats_to_disk)
        .def_readwrite("stats_history_buffer_size", &rocksdb::Options::stats_history_buffer_size)
        .def_readwrite("advise_random_on_open", &rocksdb::Options::advise_random_on_open)
        .def_readwrite("db_write_buffer_size", &rocksdb::Options::db_write_buffer_size)
        .def_readwrite("compaction_readahead_size", &rocksdb::Options::compaction_readahead_size)
        .def_readwrite("random_access_max_buffer_size", &rocksdb::Options::random_access_max_buffer_size)
        .def_readwrite("writable_file_max_buffer_size", &rocksdb::Options::writable_file_max_buffer_size)
        .def_readwrite("use_adaptive_mutex", &rocksdb::Options::use_adaptive_mutex)
        .def_readwrite("bytes_per_sync", &rocksdb::Options::bytes_per_sync)
        .def_readwrite("wal_bytes_per_sync", &rocksdb::Options::wal_bytes_per_sync)
        .def_readwrite("strict_bytes_per_sync", &rocksdb::Options::strict_bytes_per_sync)
        .def_readwrite("enable_thread_tracking", &rocksdb::Options::enable_thread_tracking)
        .def_readwrite("delayed_write_rate", &rocksdb::Options::delayed_write_rate)
        .def_readwrite("enable_pipelined_write", &rocksdb::Options::enable_pipelined_write)
        .def_readwrite("unordered_write", &rocksdb::Options::unordered_write)
        .def_readwrite("allow_concurrent_memtable_write", &rocksdb::Options::allow_concurrent_memtable_write)
        .def_readwrite("enable_write_thread_adaptive_yield", &rocksdb::Options::enable_write_thread_adaptive_yield)
        .def_readwrite("max_write_batch_group_size_bytes", &rocksdb::Options::max_write_batch_group_size_bytes)
        .def_readwrite("write_thread_max_yield_usec", &rocksdb::Options::write_thread_max_yield_usec)
        .def_readwrite("write_thread_slow_yield_usec", &rocksdb::Options::write_thread_slow_yield_usec)
        .def_readwrite("skip_stats_update_on_db_open", &rocksdb::Options::skip_stats_update_on_db_open)
        .def_readwrite("skip_checking_sst_file_sizes_on_db_open",
                       &rocksdb::Options::skip_checking_sst_file_sizes_on_db_open)
        .def_readwrite("allow_2pc", &rocksdb::Options::allow_2pc)
        .def_readwrite("fail_if_options_file_error", &rocksdb::Options::fail_if_options_file_error)
        .def_readwrite("dump_malloc_stats", &rocksdb::Options::dump_malloc_stats)
        .def_readwrite("avoid_flush_during_recovery", &rocksdb::Options::avoid_flush_during_recovery)
        .def_readwrite("avoid_flush_during_shutdown", &rocksdb::Options::avoid_flush_during_shutdown)
        .def_readwrite("allow_ingest_behind", &rocksdb::Options::allow_ingest_behind)
        .def_readwrite("two_write_queues", &rocksdb::Options::two_write_queues)
        .def_readwrite("manual_wal_flush", &rocksdb::Options::manual_wal_flush)
        .def_readwrite("atomic_flush", &rocksdb::Options::atomic_flush)
        .def_readwrite("avoid_unnecessary_blocking_io", &rocksdb::Options::avoid_unnecessary_blocking_io)
        .def_readwrite("write_dbid_to_manifest", &rocksdb::Options::write_dbid_to_manifest)
        .def_readwrite("log_readahead_size", &rocksdb::Options::log_readahead_size)
        .def_readwrite("best_efforts_recovery", &rocksdb::Options::best_efforts_recovery)
        .def_readwrite("max_bgerror_resume_count", &rocksdb::Options::max_bgerror_resume_count)
        .def_readwrite("bgerror_resume_retry_interval", &rocksdb::Options::bgerror_resume_retry_interval)
        .def_readwrite("allow_data_in_errors", &rocksdb::Options::allow_data_in_errors)
        .def_readwrite("db_host_id", &rocksdb::Options::db_host_id)
        .def_readwrite("enforce_single_del_contracts", &rocksdb::Options::enforce_single_del_contracts)
        .def("to_dict", [](const rocksdb::Options &instance) {
            return py::dict(
                "write_buffer_size"_a = instance.write_buffer_size,
                "level0_file_num_compaction_trigger"_a = instance.level0_file_num_compaction_trigger,
                "max_bytes_for_level_base"_a = instance.max_bytes_for_level_base,
                "disable_auto_compactions"_a = instance.disable_auto_compactions,
                "create_if_missing"_a = instance.create_if_missing,
                "create_missing_column_families"_a = instance.create_missing_column_families,
                "error_if_exists"_a = instance.error_if_exists, "paranoid_checks"_a = instance.paranoid_checks,
                "flush_verify_memtable_count"_a = instance.flush_verify_memtable_count,
                "track_and_verify_wals_in_manifest"_a = instance.track_and_verify_wals_in_manifest,
                "verify_sst_unique_id_in_manifest"_a = instance.verify_sst_unique_id_in_manifest,
                "max_open_files"_a = instance.max_open_files,
                "max_file_opening_threads"_a = instance.max_file_opening_threads,
                "max_total_wal_size"_a = instance.max_total_wal_size, "use_fsync"_a = instance.use_fsync,
                "db_log_dir"_a = instance.db_log_dir, "wal_dir"_a = instance.wal_dir,
                "delete_obsolete_files_period_micros"_a = instance.delete_obsolete_files_period_micros,
                "max_background_jobs"_a = instance.max_background_jobs,
                "max_background_compactions"_a = instance.max_background_compactions,
                "max_subcompactions"_a = instance.max_subcompactions,
                "max_background_flushes"_a = instance.max_background_flushes,
                "max_log_file_size"_a = instance.max_log_file_size,
                "log_file_time_to_roll"_a = instance.log_file_time_to_roll,
                "keep_log_file_num"_a = instance.keep_log_file_num,
                "recycle_log_file_num"_a = instance.recycle_log_file_num,
                "max_manifest_file_size"_a = instance.max_manifest_file_size,
                "table_cache_numshardbits"_a = instance.table_cache_numshardbits,
                "WAL_ttl_seconds"_a = instance.WAL_ttl_seconds, "WAL_size_limit_MB"_a = instance.WAL_size_limit_MB,
                "manifest_preallocation_size"_a = instance.manifest_preallocation_size,
                "allow_mmap_reads"_a = instance.allow_mmap_reads, "allow_mmap_writes"_a = instance.allow_mmap_writes,
                "use_direct_reads"_a = instance.use_direct_reads,
                "use_direct_io_for_flush_and_compaction"_a = instance.use_direct_io_for_flush_and_compaction,
                "allow_fallocate"_a = instance.allow_fallocate, "is_fd_close_on_exec"_a = instance.is_fd_close_on_exec,
                "stats_dump_period_sec"_a = instance.stats_dump_period_sec,
                "stats_persist_period_sec"_a = instance.stats_persist_period_sec,
                "persist_stats_to_disk"_a = instance.persist_stats_to_disk,
                "stats_history_buffer_size"_a = instance.stats_history_buffer_size,
                "advise_random_on_open"_a = instance.advise_random_on_open,
                "db_write_buffer_size"_a = instance.db_write_buffer_size,
                "compaction_readahead_size"_a = instance.compaction_readahead_size,
                "random_access_max_buffer_size"_a = instance.random_access_max_buffer_size,
                "writable_file_max_buffer_size"_a = instance.writable_file_max_buffer_size,
                "use_adaptive_mutex"_a = instance.use_adaptive_mutex, "bytes_per_sync"_a = instance.bytes_per_sync,
                "wal_bytes_per_sync"_a = instance.wal_bytes_per_sync,
                "strict_bytes_per_sync"_a = instance.strict_bytes_per_sync,
                "enable_thread_tracking"_a = instance.enable_thread_tracking,
                "delayed_write_rate"_a = instance.delayed_write_rate,
                "enable_pipelined_write"_a = instance.enable_pipelined_write,
                "unordered_write"_a = instance.unordered_write,
                "allow_concurrent_memtable_write"_a = instance.allow_concurrent_memtable_write,
                "enable_write_thread_adaptive_yield"_a = instance.enable_write_thread_adaptive_yield,
                "max_write_batch_group_size_bytes"_a = instance.max_write_batch_group_size_bytes,
                "write_thread_max_yield_usec"_a = instance.write_thread_max_yield_usec,
                "write_thread_slow_yield_usec"_a = instance.write_thread_slow_yield_usec,
                "skip_stats_update_on_db_open"_a = instance.skip_stats_update_on_db_open,
                "skip_checking_sst_file_sizes_on_db_open"_a = instance.skip_checking_sst_file_sizes_on_db_open,
                "allow_2pc"_a = instance.allow_2pc,
                "fail_if_options_file_error"_a = instance.fail_if_options_file_error,
                "dump_malloc_stats"_a = instance.dump_malloc_stats,
                "avoid_flush_during_recovery"_a = instance.avoid_flush_during_recovery,
                "avoid_flush_during_shutdown"_a = instance.avoid_flush_during_shutdown,
                "allow_ingest_behind"_a = instance.allow_ingest_behind,
                "two_write_queues"_a = instance.two_write_queues, "manual_wal_flush"_a = instance.manual_wal_flush,
                "atomic_flush"_a = instance.atomic_flush,
                "avoid_unnecessary_blocking_io"_a = instance.avoid_unnecessary_blocking_io,
                "write_dbid_to_manifest"_a = instance.write_dbid_to_manifest,
                "log_readahead_size"_a = instance.log_readahead_size,
                "best_efforts_recovery"_a = instance.best_efforts_recovery,
                "max_bgerror_resume_count"_a = instance.max_bgerror_resume_count,
                "bgerror_resume_retry_interval"_a = instance.bgerror_resume_retry_interval,
                "allow_data_in_errors"_a = instance.allow_data_in_errors, "db_host_id"_a = instance.db_host_id,
                "enforce_single_del_contracts"_a = instance.enforce_single_del_contracts);
        });

    //RocksDB Status aka rocksdb::Status
    py::class_<rocksdb::Status>(m, "Status")
        .def(py::init())
        .def_property_readonly("ok", &rocksdb::Status::ok)
        .def_property_readonly("message", &rocksdb::Status::getState)
        .def_property_readonly("is_ok_overwritten", &rocksdb::Status::IsOkOverwritten)
        .def_property_readonly("is_not_found", &rocksdb::Status::IsNotFound)
        .def_property_readonly("is_corruption", &rocksdb::Status::IsCorruption)
        .def_property_readonly("is_not_supported", &rocksdb::Status::IsNotSupported)
        .def_property_readonly("is_invalid_argument", &rocksdb::Status::IsInvalidArgument)
        .def_property_readonly("is_io_error", &rocksdb::Status::IsIOError)
        .def_property_readonly("is_merge_in_progress", &rocksdb::Status::IsMergeInProgress)
        .def_property_readonly("is_incomplete", &rocksdb::Status::IsIncomplete)
        .def_property_readonly("is_shutdown_in_progress", &rocksdb::Status::IsShutdownInProgress)
        .def_property_readonly("is_timed_out", &rocksdb::Status::IsTimedOut)
        .def_property_readonly("is_aborted", &rocksdb::Status::IsAborted)
        .def_property_readonly("is_lock_limit", &rocksdb::Status::IsLockLimit)
        .def_property_readonly("is_busy", &rocksdb::Status::IsBusy)
        .def_property_readonly("is_deadlock", &rocksdb::Status::IsDeadlock)
        .def_property_readonly("is_expired", &rocksdb::Status::IsExpired)
        .def_property_readonly("is_try_again", &rocksdb::Status::IsTryAgain)
        .def_property_readonly("is_compaction_too_large", &rocksdb::Status::IsCompactionTooLarge)
        .def_property_readonly("is_column_family_dropped", &rocksdb::Status::IsColumnFamilyDropped)
        .def_property_readonly("is_no_space", &rocksdb::Status::IsNoSpace)
        .def_property_readonly("is_memory_limit", &rocksdb::Status::IsMemoryLimit)
        .def_property_readonly("is_path_not_found", &rocksdb::Status::IsPathNotFound)
        .def_property_readonly("is_manual_compaction_paused", &rocksdb::Status::IsManualCompactionPaused)
        .def_property_readonly("is_txn_not_prepared", &rocksdb::Status::IsTxnNotPrepared)
        .def_property_readonly("is_io_fenced", &rocksdb::Status::IsIOFenced)
        .def("__str__", &rocksdb::Status::ToString)
        .def("__repr__", &rocksdb::Status::ToString)
        .def("to_dict", [](const rocksdb::Status &instance) {
            return py::dict(
                "ok"_a = instance.ok(), "message"_a = instance.getState(),
                "is_ok_overwritten"_a = instance.IsOkOverwritten(), "is_not_found"_a = instance.IsNotFound(),
                "is_corruption"_a = instance.IsCorruption(), "is_not_supported"_a = instance.IsNotSupported(),
                "is_invalid_argument"_a = instance.IsInvalidArgument(), "is_io_error"_a = instance.IsIOError(),
                "is_merge_in_progress"_a = instance.IsMergeInProgress(), "is_incomplete"_a = instance.IsIncomplete(),
                "is_shutdown_in_progress"_a = instance.IsShutdownInProgress(), "is_timed_out"_a = instance.IsTimedOut(),
                "is_aborted"_a = instance.IsAborted(), "is_lock_limit"_a = instance.IsLockLimit(),
                "is_busy"_a = instance.IsBusy(), "is_deadlock"_a = instance.IsDeadlock(),
                "is_expired"_a = instance.IsExpired(), "is_try_again"_a = instance.IsTryAgain(),
                "is_compaction_too_large"_a = instance.IsCompactionTooLarge(),
                "is_column_family_dropped"_a = instance.IsColumnFamilyDropped(), "is_no_space"_a = instance.IsNoSpace(),
                "is_memory_limit"_a = instance.IsMemoryLimit(), "is_path_not_found"_a = instance.IsPathNotFound(),
                "is_manual_compaction_paused"_a = instance.IsManualCompactionPaused(),
                "is_txn_not_prepared"_a = instance.IsTxnNotPrepared(), "is_io_fenced"_a = instance.IsIOFenced());
        });

    // RocksDB ReadOptions aka rocksdb::ReadOptions
    py::class_<rocksdb::ReadOptions, std::shared_ptr<rocksdb::ReadOptions>>(m, "_ReadOptions")
        .def(py::init())
        .def_readwrite("readahead_size", &rocksdb::ReadOptions::readahead_size)
        .def_readwrite("max_skippable_internal_keys", &rocksdb::ReadOptions::max_skippable_internal_keys)
        .def_readwrite("verify_checksums", &rocksdb::ReadOptions::verify_checksums)
        .def_readwrite("fill_cache", &rocksdb::ReadOptions::fill_cache)
        .def_readwrite("tailing", &rocksdb::ReadOptions::tailing)
        .def_readwrite("total_order_seek", &rocksdb::ReadOptions::total_order_seek)
        .def_readwrite("auto_prefix_mode", &rocksdb::ReadOptions::auto_prefix_mode)
        .def_readwrite("prefix_same_as_start", &rocksdb::ReadOptions::prefix_same_as_start)
        .def_readwrite("pin_data", &rocksdb::ReadOptions::pin_data)
        .def_readwrite("background_purge_on_iterator_cleanup",
                       &rocksdb::ReadOptions::background_purge_on_iterator_cleanup)
        .def_readwrite("ignore_range_deletions", &rocksdb::ReadOptions::ignore_range_deletions)
        .def_readwrite("value_size_soft_limit", &rocksdb::ReadOptions::value_size_soft_limit)
        .def_readwrite("adaptive_readahead", &rocksdb::ReadOptions::adaptive_readahead)
        .def_readwrite("async_io", &rocksdb::ReadOptions::async_io)
        .def_readwrite("optimize_multiget_for_io", &rocksdb::ReadOptions::optimize_multiget_for_io);

    // RocksDB WriteOptions aka rocksdb::WriteOptions
    py::class_<rocksdb::WriteOptions, std::shared_ptr<rocksdb::WriteOptions>>(m, "_WriteOptions")
        .def(py::init())
        .def_readwrite("sync", &rocksdb::WriteOptions::sync)
        .def_readwrite("disableWAL", &rocksdb::WriteOptions::disableWAL)
        .def_readwrite("ignore_missing_column_families", &rocksdb::WriteOptions::ignore_missing_column_families)
        .def_readwrite("no_slowdown", &rocksdb::WriteOptions::no_slowdown)
        .def_readwrite("low_pri", &rocksdb::WriteOptions::low_pri)
        .def_readwrite("memtable_insert_hint_per_batch", &rocksdb::WriteOptions::memtable_insert_hint_per_batch)
        .def_readwrite("protection_bytes_per_key", &rocksdb::WriteOptions::protection_bytes_per_key);

    // RocksDB FlushOptions aka rocksdb::ReadOptions
    py::class_<rocksdb::FlushOptions, std::shared_ptr<rocksdb::FlushOptions>>(m, "_FlushOptions")
        .def(py::init())
        .def_readwrite("wait", &rocksdb::FlushOptions::wait)
        .def_readwrite("allow_write_stall", &rocksdb::FlushOptions::allow_write_stall);
}
