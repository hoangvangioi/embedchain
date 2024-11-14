use bytes::Bytes;
use coap::client::UdpCoAPClient;
use coap::Server;
use coap_lite::{CoapRequest, RequestType as Method};
use lazy_static::lazy_static;
use prost::Message;
use std::collections::HashMap;
use std::ffi::{CStr, CString};
use std::net::SocketAddr;
use std::sync::{Arc, Mutex, RwLock};
use tokio::runtime::Runtime;

struct SensorDataStorage {
    data: HashMap<String, sensor::SensorData>,
}

lazy_static! {
    static ref SENSOR_STORAGE: Arc<Mutex<SensorDataStorage>> =
        Arc::new(Mutex::new(SensorDataStorage {
            data: HashMap::new()
        }));
    static ref OLD_TIMESTAMPS: Arc<RwLock<HashMap<String, Option<u64>>>> =
        Arc::new(RwLock::new(HashMap::new()));
}

pub mod sensor {
    include!(concat!(env!("OUT_DIR"), "/sensor.rs"));
}

#[no_mangle]
pub extern "C" fn send_sensor_data(url: *const i8, timestamp: u64, data: *const u8) {
    // Convert the URL from C string to Rust string
    let url_str = unsafe { CStr::from_ptr(url).to_str().unwrap() };

    // Determine the length of the data byte array
    let data_length = unsafe {
        let mut len = 0;
        while *data.add(len) != 0 {
            len += 1;
        }
        len
    };

    // Create a Rust slice from the C byte array
    let data_slice: &[u8] = unsafe { std::slice::from_raw_parts(data, data_length) };

    // Create the sensor data structure using the raw byte array directly
    let sensor_data = sensor::SensorData {
        timestamp,
        data: data_slice.to_vec(), // Convert to Vec<u8> for protobuf
    };

    let sensor_network = sensor::SensorNetwork {
        sensors: vec![sensor_data],
    };

    // Encode the sensor network data to protobuf
    let encoded = sensor_network.encode_to_vec();

    // Use the async block to send data
    let _ = Runtime::new().unwrap().block_on(async {
        match UdpCoAPClient::post(url_str, encoded).await {
            Ok(response) => {
                println!(
                    "Server reply: {}",
                    String::from_utf8(response.message.payload).unwrap()
                );
            }
            Err(e) => {
                eprintln!("Request error: {:?}", e);
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn start_server(addr: *const i8) {
    let addr_str = unsafe { CStr::from_ptr(addr).to_str().unwrap() };

    Runtime::new().unwrap().block_on(async move {
        let server = Server::new_udp(addr_str).unwrap();
        println!("Server up on {}", addr_str);

        server
            .run(|mut request: Box<CoapRequest<SocketAddr>>| async move {
                match request.get_method() {
                    &Method::Post => {
                        let payload = request.message.payload.clone();
                        let decoded = sensor::SensorNetwork::decode(Bytes::from(payload))
                            .expect("Failed to decode sensor data");

                        let mut storage = SENSOR_STORAGE.lock().unwrap();
                        let path = request.get_path().to_string();

                        // Nếu có dữ liệu mới, cập nhật lại dữ liệu cho URL
                        if let Some(sensor_data) = decoded.sensors.first() {
                            storage.data.insert(path.clone(), sensor_data.clone());
                        }

                        if let Some(ref mut message) = request.response {
                            message.message.payload = b"Data received".to_vec();
                        }
                    }
                    _ => println!("Request by other method"),
                };

                return request;
            })
            .await
            .unwrap();
    });
}

#[repr(C)]
pub struct SensorData {
    timestamp: u64,
    data: *const i8, // Pointer to the encoded data string
}

#[no_mangle]
pub extern "C" fn free_sensor_data(data: *const sensor::SensorData) {
    if !data.is_null() {
        unsafe {
            let _ = Box::from_raw(data as *mut sensor::SensorData);
        }
    }
}

#[no_mangle]
pub extern "C" fn retrieve_sensor_data(url: *const i8, data: *mut SensorData) -> bool {
    let url_str: &str = unsafe { CStr::from_ptr(url).to_str().unwrap() };
    let storage = SENSOR_STORAGE.lock().unwrap();

    // Check if there's data for the URL
    if let Some(sensor_data) = storage.data.get(url_str) {
        let mut old_timestamps = OLD_TIMESTAMPS.write().unwrap();

        // Get the old timestamp for this URL
        let old_timestamp = old_timestamps.entry(url_str.to_string()).or_insert(None);

        // Check if the current timestamp is the same as the old one
        if *old_timestamp == Some(sensor_data.timestamp) {
            return false;
        }

        unsafe {
            (*data).timestamp = sensor_data.timestamp;
            let c_string =
                CString::new(sensor_data.data.clone()).expect("Failed to create CString");
            (*data).data = c_string.into_raw();
            // Update the old timestamp for this URL
            *old_timestamp = Some(sensor_data.timestamp);
        }
        return true;
    }

    false
}
